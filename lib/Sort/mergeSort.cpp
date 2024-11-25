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

void AEPKSS::Sort::merge_sort_parallel(vector<size_t> &in, size_t concurrency)
{

    // create thread pool
    auto threadPool = AEPKSS::Util::ThreadPool(concurrency);
    threadPool.start();

    // perform merge sort
    binary_semaphore sem{1};
    split_parallel(in, 0, in.size() - 1, &sem, threadPool);

    // create background task to observe progress
    // as main thread can not aquire semaphores
    auto poolObserver = thread([pool = &threadPool, sem = &sem]
                               {
        sem->acquire();
        pool->stop();
        sem->release(); });
    poolObserver.join();
}

static void split_parallel(vector<size_t> &in, size_t left, size_t right, binary_semaphore *sem, AEPKSS::Util::ThreadPool &pool)
{
    // end reached, stop splitting
    if (left >= right)
        return;

    // acquire lock
    sem->acquire();

    // Calculate the midpoint
    int middle = left + ((right - left) / 2);

    if (MERGE_SORT_DEBUG)
        cout << "\t\tsplit - l: " << left << " | m: " << middle << " | r: " << right << endl;

    // create semaphores
    binary_semaphore semL{1}, semR{1};

    // Splitting
    split_parallel(in, left, middle, &semL, pool);
    split_parallel(in, middle + 1, right, &semR, pool);

    cout << "\t\tlambda - l: " << left << " | m: " << middle << " | r: " << right << endl;
    const auto func = [sem = sem, semL = &semL, semR = &semR, in = &in, left = left, middle = middle, right = right]
    {   
        cout << "\t\tdbg - l: " << left << " | m: " << middle << " | r: " << right << endl;

        // create lock in job scope
        semL->acquire();
        semR->acquire();

        // perform merge
        //merge_classic(*in, left, right, middle);

        // unlock all
        semL->release();
        semR->release();
        sem->release(); };
    // func();
    pool.submit(func);
}

static void split(vector<size_t> &in, size_t left, size_t right, AEPKSS::Sort::MergeStrategy strategy)
{
    // end reached, stop splitting
    if (left >= right)
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
