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

    priority_queue<ParallelQuickSortReturn, vector<ParallelQuickSortReturn>, ParallelQuickSortReturnComparatorLeft> queueLeft;
    priority_queue<ParallelQuickSortReturn, vector<ParallelQuickSortReturn>, ParallelQuickSortReturnComparatorRight> queueRight;
    vector<size_t> out;

    for (auto task : cache)
    {
        auto result = task.get();
        queueLeft.push(result);
        queueRight.push(result);
    }

    cout << "Pivot: " << pivot << endl;
    size_t inOffset = 0;

    while (!queueLeft.empty())
    {
        auto x = queueLeft.top();
        auto count = x.countLeft;
        if (count == 0)
            continue;

        auto begin = x.in->begin();
        copy(begin, begin + count, in.begin() + inOffset);
        inOffset += count;

        queueLeft.pop();
    }

    in[inOffset] = pivot;
    inOffset++;

    while (!queueRight.empty())
    {
        auto x = queueRight.top();
        auto count = x.countRight;
        if (count == 0)
            continue;

        auto end = x.in->end();
        copy(end - count, end, in.begin() + inOffset);
        inOffset += count;

        queueRight.pop();
    }

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
    if (in.size() < 2)
        return {
            .in = &in,
            .prefixSumLeft = in[0] < pivot ? in[0] : 0,
            .prefixSumRight = in[0] > pivot ? in[0] : 0,
            .countLeft = (size_t)(in[0] < pivot ? 1 : 0),
            .countRight = (size_t)(in[0] > pivot ? 1 : 0),
        };

    AEPKSS::Sort::ParallelQuickSortReturn ret;

    // preliminary sorting
    AEPKSS::Sort::quick_sort(in);
    ret.in = &in;

    for (auto x : in)
    {
        if (x < pivot)
        {
            ret.countLeft++;
            ret.prefixSumLeft += x;
        }
        else if (x > pivot)
        {
            ret.countRight++;
            ret.prefixSumRight += x;
        }
    }

    return ret;
}