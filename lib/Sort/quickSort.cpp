#include "quickSort.h"

void AEPKSS::Sort::quick_sort(vector<size_t> &in)
{
    sort(in, 0, in.size() - 1);
}

size_t AEPKSS::Sort::quick_sort_parallel(vector<size_t> &in, size_t concurrency)
{
    size_t n = in.size();
    size_t blockSize = n / concurrency;

    if (n < concurrency || concurrency <= 1 || blockSize <= 1)
    {
        quick_sort(in);
        return 1;
    }

    auto pivotIndex = in.size() / 2;
    auto pivot = in[pivotIndex];

    vector<shared_future<ParallelQuickSortReturn>> cache;

    // create job for each processor
    for (auto i = 0; i < concurrency; i++)
    {
        auto start = i * blockSize;
        auto end = start + blockSize;

        vector<size_t> bucket;

        // create buckets
        if (i + 1 == concurrency)
            bucket = vector<size_t>(in.begin() + start, in.end());
        else
            bucket = vector<size_t>(in.begin() + start, in.begin() + end);

        const auto func = bind(sort_parallel, bucket, pivot);
        shared_future<ParallelQuickSortReturn> future = async(launch::async, func);

        cache.emplace_back(future);
    }

    const bool isLeft = true;
    const bool isNotLeft = false;

    const auto positionLeft = bind(position_by_prefixsum, cache, true);
    shared_future<vector<size_t>> futureLeft = async(launch::async, positionLeft);

    const auto positionRight = bind(position_by_prefixsum, cache, false);
    shared_future<vector<size_t>> futureRight = async(launch::async, positionRight);

    auto leftSide = futureLeft.get();
    copy(leftSide.begin(), leftSide.end(), in.begin());
    in[leftSide.size()] = pivot;
    auto rightSide = futureRight.get();
    copy(rightSide.begin(), rightSide.end(), in.begin() + leftSide.size() + 1);

    return concurrency;
}

static void sort(vector<size_t> &in, size_t left, size_t right)
{
    if (left >= right)
        return;

    auto pIndex = partition(in, left, right);
    if (pIndex > 0)
        sort(in, left, pIndex - 1);
    sort(in, pIndex + 1, right);
}

static size_t partition(vector<size_t> &in, size_t left, size_t right)
{
    auto pivot = in[right];
    size_t swapIndex = left;

    for (auto i = left; i < right; i++)
    {
        if (in[i] > pivot)
            continue;

        swap(in[swapIndex++], in[i]);
    }

    swap(in[swapIndex], in[right]);

    return swapIndex;
}

static AEPKSS::Sort::ParallelQuickSortReturn sort_parallel(vector<size_t> &in, size_t pivot)
{
    AEPKSS::Sort::ParallelQuickSortReturn ret;

    // preliminary sorting
    AEPKSS::Sort::quick_sort(in);
    ret.in = &in;

    size_t lastSumLeft = 0, lastSumRight = 0;

    for (auto x : in)
    {
        if (x < pivot)
        {
            lastSumLeft += x;
            ret.prefixSumLeft.emplace_back(lastSumLeft);
        }
        else if (x > pivot)
        {
            lastSumRight += x;
            ret.prefixSumRight.emplace_back(lastSumRight);
        }
    }

    ret.countLeft = ret.prefixSumLeft.size();
    ret.countRight = ret.prefixSumRight.size();

    return ret;
}

static vector<size_t> position_by_prefixsum(vector<shared_future<AEPKSS::Sort::ParallelQuickSortReturn>> &cache, bool &isLeft)
{
    priority_queue<pair<size_t, size_t>, vector<pair<size_t, size_t>>, AEPKSS::Sort::ParallelQuickSortPairComparator> queue;

    for (auto task : cache)
    {
        auto result = task.get();
        auto in = *result.in;

        auto startIndex = isLeft ? 0 : (in.size() - result.countRight);
        for (auto i : (isLeft ? result.prefixSumLeft : result.prefixSumRight))
        {
            // cout << "I: " << i << " | V: " << in[startIndex] << endl;
            queue.push({i, in[startIndex]});
            startIndex++;
        }
    }

    vector<size_t> out;
    out.reserve(queue.size());

    while (!queue.empty())
    {
        // cout << "Q: " << queue.top().first << " - " << queue.top().second << endl;
        out.emplace_back(queue.top().second);
        queue.pop();
    }

    // FIXME: honestly this should now be sorted but in fact it is not for e.g. 11 values and 2 threads
    // either i do not understand prefix sums or there is something else off here
    // to at least get the algorithm to run let's do the following:
    std::sort(out.begin(), out.end());

    return out;
}