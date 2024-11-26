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

    size_t n = in.size();
    size_t blockSize = n / concurrency;

    if (n < concurrency || concurrency <= 1 || blockSize <= 1)
    {
        split(in, 0, in.size() - 1, AEPKSS::Sort::MergeStrategy::Classic);
        return 1;
    }

    vector<shared_future<vector<size_t>>> cache;
    vector<shared_future<vector<size_t>>> intermediateCache;

    for (auto i = 0; i < concurrency; i++)
    {
        auto start = i * blockSize;
        auto end = start + blockSize;

        vector<size_t> bucket;
        vector<size_t> out;
        binary_semaphore sem{0};

        // create buckets
        if (i + 1 == concurrency)
            bucket = vector<size_t>(in.begin() + start, in.end());
        else
            bucket = vector<size_t>(in.begin() + start, in.begin() + end);

        const auto func = bind(parallell_merge_per_core, bucket);
        shared_future<vector<size_t>> future = async(launch::async, func);

        intermediateCache.emplace_back(future);
    }

    while (intermediateCache.size() > 1)
    {
        cache.insert(cache.end(), intermediateCache.begin(), intermediateCache.end());
        vector<shared_future<vector<size_t>>> tmpCache = intermediateCache;
        intermediateCache.clear();

        for (size_t i = 0, j = tmpCache.size(); i < j; i += 2)
        {
            auto left = tmpCache[i];
            optional<shared_future<vector<size_t>>> right = nullopt;
            if ((i + 1) < j)
                right = tmpCache[i + 1];

            const auto func = bind(parallell_merge_block, left, right);
            shared_future<vector<size_t>> future = async(launch::async, func);
            intermediateCache.emplace_back(future);
        }
    }

    if (MERGE_SORT_DEBUG)
        for (auto x : cache)
        {
            auto y = x.get();
            cout << "Is Sorted: " << is_sorted(y.cbegin(), y.cend()) << endl;
        }

    in = cache.back().get();

    return cache.size();
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

static vector<size_t> parallell_merge_per_core(vector<size_t> &in)
{
    split(in, 0, in.size() - 1, AEPKSS::Sort::MergeStrategy::Classic);
    return in;
}

static vector<size_t> parallell_merge_block(shared_future<vector<size_t>> &left, optional<shared_future<vector<size_t>>> &right)
{
    if (!right.has_value())
        return left.get();

    auto l = left.get();
    auto r = right.value().get();
    return merge_parallel(l, r);
}
