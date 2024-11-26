#include "mergeSort.h"

void AEPKSS::Sort::merge_sort(vector<size_t> &in)
{
    split(in, 0, in.size() - 1, MergeStrategy::Classic);
}

void AEPKSS::Sort::merge_sort(vector<size_t> &in, MergeStrategy strategy)
{
    if (strategy != MergeStrategy::Parallel)
        split(in, 0, in.size() - 1, strategy);
    else
        merge_sort_parallel(in, thread::hardware_concurrency());
}

size_t AEPKSS::Sort::merge_sort_parallel(vector<size_t> &in, size_t concurrency)
{

    // create thread pool
    auto threadPool = AEPKSS::Util::ThreadPool(concurrency);
    threadPool.start();

    // perform merge sort
    binary_semaphore sem{0};
    split_parallel_v3(in, sem, threadPool);

    // create background task to observe progress
    // as main thread can not aquire semaphores
    auto poolObserver = thread([&]
                               {                 
        while(threadPool.isBusy()) this_thread::yield();
        threadPool.stop(); });
    poolObserver.join();

    return threadPool.jobsProcessed;
}

static binary_semaphore *split_parallel_v3(vector<size_t> &in, binary_semaphore &sem, AEPKSS::Util::ThreadPool &pool)
{
    size_t n = in.size();
    size_t blockSize = n / pool.concurrency;

    if (n < pool.concurrency || blockSize < 1)
    {
        split(in, 0, in.size() - 1, AEPKSS::Sort::MergeStrategy::Classic);
        sem.release();
        return &sem;
    }

    vector<pair<binary_semaphore *, vector<size_t> *>> cache;

    for (auto i = 0; i < pool.concurrency; i++)
    {
        auto start = i * blockSize;
        auto end = start + blockSize;

        vector<size_t> bucket;
        vector<size_t> out;
        binary_semaphore sem{0};

        if (i + 1 == pool.concurrency)
            bucket = vector<size_t>(in.begin() + start, in.end());
        else
            bucket = vector<size_t>(in.begin() + start, in.begin() + end);

        const auto func = std::bind(split_parallel_v3_helper, ref(bucket), ref(sem));
        pool.submit(func);

        pair<binary_semaphore *, vector<size_t> *> cachePair = {&sem, &bucket};
        cache.emplace_back(cachePair);

        sem.acquire();
        cout << "S: " << bucket.size() << " | s: " << is_sorted(bucket.cbegin(), bucket.cend()) << endl;
    }

    // TODO: we now have our buckets, next we need to merge them together

    sem.release();
    return &sem;
}

static void split_parallel_v3_helper(vector<size_t> &in, binary_semaphore &sem)
{
    split(in, 0, in.size() - 1, AEPKSS::Sort::MergeStrategy::Classic);
    sem.release();
}

static binary_semaphore *split_parallel_v2(vector<size_t> &in, binary_semaphore &sem, AEPKSS::Util::ThreadPool &pool)
{
    size_t middle = in.size() / 2;
    if (middle == 0)
    {
        sem.release();
        return &sem;
    }

    if (in.size() < MERGE_SORT_PARALLEL_THRESHOLD)
    {
        split(in, 0, in.size() - 1, AEPKSS::Sort::Classic);
        sem.release();
        return &sem;
    }

    binary_semaphore semL{0};
    binary_semaphore semR{0};

    // create copies to work with
    vector<size_t> left(in.begin(), in.begin() + middle);
    vector<size_t> right(in.begin() + middle, in.end());

    // Sort first and second halves
    auto semLR = split_parallel_v2(left, semL, pool);
    auto semRR = split_parallel_v2(right, semR, pool);

    semLR->acquire();
    semRR->acquire();

    const auto func = [=, sem = &sem, in = &in] mutable
    {
        auto out = merge_parallel(left, right);
        *in = move(out);
        sem->release();
    };
    pool.submit(func);

    return &sem;
}

static void split_parallel(vector<size_t> &in, size_t left, size_t right, size_t semId, AEPKSS::Util::ThreadPool &pool)
{
    // end reached, stop splitting
    if (left >= right)
        return;

    // acquire lock
    SemaphoreTracker::getInstance()->get(semId)->acquire();

    // Calculate the midpoint
    int middle = left + ((right - left) / 2);

    if (MERGE_SORT_DEBUG)
        cout << "\t\tsplit - l: " << left << " | m: " << middle << " | r: " << right << endl;

    // create semaphores
    binary_semaphore semL{1}, semR{1};
    size_t semIdL = SemaphoreTracker::getInstance()->track(&semL);
    size_t semIdR = SemaphoreTracker::getInstance()->track(&semR);

    // Splitting
    split_parallel(in, left, middle, semIdL, pool);
    split_parallel(in, middle + 1, right, semIdR, pool);

    if (MERGE_SORT_DEBUG)
        cout << "\t\tlambda - l: " << left << " | m: " << middle << " | r: " << right << endl;

    const auto func = [semId = semId, semIdL = semIdL, semIdR = semIdR, in = &in, left = left, middle = middle, right = right]
    {
        if (MERGE_SORT_DEBUG)
            cout << "\t\tdbg - l: " << left << " | m: " << middle << " | r: " << right << endl;

        // create lock in job scope
        SemaphoreTracker::getInstance()->get(semIdL)->acquire();
        SemaphoreTracker::getInstance()->get(semIdR)->acquire();

        // perform merge
        merge_classic(*in, left, right, middle);

        // unlock all
        SemaphoreTracker::getInstance()->get(semIdL)->release();
        SemaphoreTracker::getInstance()->get(semIdR)->release();
        SemaphoreTracker::getInstance()->get(semId)->release();

        if (MERGE_SORT_DEBUG)
            cout << "\t\tdbgend - l: " << left << " | m: " << middle << " | r: " << right << endl;
    };
    // func();
    pool.submit(func);
}

static void split(vector<size_t> &in, size_t left, size_t right, AEPKSS::Sort::MergeStrategy strategy)
{
    // end reached, stop splitting
    if (in.size() <= 1 || left >= right)
        return;

    // Calculate the midpoint
    int middle = left + ((right - left) / 2);

    if (MERGE_SORT_DEBUG)
        cout << "\t\tsplit - l: " << left << " | m: " << middle << " | r: " << right << endl;

    // Sort first and second halves
    split(in, left, middle, strategy);
    split(in, middle + 1, right, strategy);

    // Merge the sorted halves
    switch (strategy)
    {
    case AEPKSS::Sort::MergeStrategy::Classic:
        merge_classic(in, left, right, middle);
        break;
    case AEPKSS::Sort::MergeStrategy::InMemory:
        merge_in_memory(in, left, right, middle);
        break;
    }
}

static void merge_in_memory(vector<size_t> &in, size_t left, size_t right, size_t middle)
{
    if (MERGE_SORT_DEBUG)
        cout << "\tmerge - l: " << left << " | m: " << middle << " | r: " << right << endl;

    auto pL = left;
    auto pM = middle + 1;

    size_t x, y;
    optional<size_t> tmp = nullopt;

    for (; pL < pM; pL++)
    {
        // right side sorted, only left remains
        if (pM > right)
            break;

        x = in[pL];
        y = in[pM];

        if (MERGE_SORT_DEBUG)
            cout << "\t\tcmp [" << pL << "," << pM << "] as (" << x << (x < y ? " < " : " > ") << y << ")" << endl;

        if (y < x)
        {
            if (MERGE_SORT_DEBUG)
                cout << "\t\tcmp y > x" << endl;

            // shift data right by 1
            for (auto i = pM; i > pL; i--)
            {
                in[i] = in[i - 1];
            }

            // shift middle pointer right by 1
            pM++;

            // assign value
            in[pL] = y;
        }
    }
}

static void merge_classic(vector<size_t> &in, size_t left, size_t right, size_t middle)
{
    if (MERGE_SORT_DEBUG)
        cout << "\t\tmerge - l: " << left << " | m: " << middle << " | r: " << right << endl;

    auto pL = left;
    auto pM = middle + 1;

    auto sizeLeft = pM - left;
    auto sizeRight = right - middle;

    queue<size_t> buffLeft, buffRight;

    // populate buffers
    for (auto i = 0; i < sizeLeft; i++)
        buffLeft.push(in[left + i]);
    for (auto i = 0; i < sizeRight; i++)
        buffRight.push(in[pM + i]);

    size_t blV, brV;

    // merge both buffers into input array
    while (!buffLeft.empty() && !buffRight.empty())
    {
        blV = buffLeft.front();
        brV = buffRight.front();

        if (blV <= brV)
        {
            in[pL] = blV;
            buffLeft.pop();
        }
        else
        {
            in[pL] = brV;
            buffRight.pop();
        }

        pL++;
    }

    // empty buffers
    while (!buffLeft.empty())
    {
        in[pL] = buffLeft.front();
        buffLeft.pop();
        pL++;
    }
    while (!buffRight.empty())
    {
        in[pL] = buffRight.front();
        buffRight.pop();
        pL++;
    }
}

static vector<size_t> merge_parallel(vector<size_t> &left, vector<size_t> &right)
{
    size_t pL = 0, pR = 0, pO = 0;
    int64_t sL = left.size(), sR = right.size();

    vector<size_t> out;
    out.reserve(sL + sR);

    while (pL < sL && pR < sR)
    {
        if (left[pL] <= right[pR])
            out[pO++] = left[pL++];
        else
            out[pO++] = right[pR++];
    }

    while (pL < sL)
        out[pO++] = left[pL++];

    while (pR < sR)
        out[pO++] = right[pR++];

    return out;
}