#include "quickSort.h"

void AEPKSS::Sort::quick_sort(vector<size_t> &in)
{
    sort(in, 0, in.size() - 1);
}

size_t AEPKSS::Sort::quick_sort_parallel(vector<size_t> &in, size_t concurrency)
{
    size_t n = in.size();
    size_t blockSize = n / concurrency;

    if (n < concurrency || concurrency <= 1 || blockSize <= QUICK_SORT_THRESHOLD)
    {
        quick_sort(in);
        return 1;
    }

    return partition_parallel(in, concurrency);
}

static void sort(vector<size_t> &in, long long left, long long right)
{
    if (left >= right)
        return;

    auto pIndex = partition(in, left, right);
    if (pIndex > 0)
        sort(in, left, pIndex - 1);
    sort(in, pIndex + 1, right);
}

static size_t partition(vector<size_t> &in, long long left, long long right)
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

static size_t partition_parallel(vector<size_t> &in, size_t concurrency)
{
    size_t n = in.size();
    size_t blockSize = n / concurrency;
    size_t tasks = concurrency;

    if (n < concurrency || concurrency <= 1 || blockSize <= QUICK_SORT_THRESHOLD)
    {
        AEPKSS::Sort::quick_sort(in);
        return 1;
    }

    auto pivotIndex = in.size() / 2;
    auto pivot = in[pivotIndex];
    vector<shared_future<AEPKSS::Sort::ParallelQuickSortReturn>> cache;

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
        shared_future<AEPKSS::Sort::ParallelQuickSortReturn> future = async(launch::async, func);

        cache.emplace_back(future);
    }

    vector<size_t> buffer;
    for (auto task : cache)
    {
        auto result = task.get();
        auto begin = result.in->begin();
        buffer.insert(buffer.end(), begin, begin + result.countLeft);
    }
    tasks += partition_parallel(buffer, concurrency);

    // merge left into input
    copy(buffer.begin(), buffer.end(), in.begin());
    // place pivot element
    pivotIndex = buffer.size();
    in[pivotIndex] = pivot;

    buffer.clear();
    for (auto task : cache)
    {
        auto result = task.get();
        auto end = result.in->end();
        buffer.insert(buffer.end(), end - result.countRight, end);
    }
    tasks += partition_parallel(buffer, concurrency);

    // merge right into input
    copy(buffer.begin(), buffer.end(), in.begin() + pivotIndex + 1);
    return tasks;
}

static AEPKSS::Sort::ParallelQuickSortReturn sort_parallel(vector<size_t> &in, size_t pivot)
{
    AEPKSS::Sort::ParallelQuickSortReturn ret;
    ret.in = &in;

    for (long long left = 0, right = in.size() - 1; left <= right; left++)
    {
        if (in[left] < pivot)
        {
            ret.countLeft++;
        }
        else if (in[left] > pivot)
        {
            ret.countRight++;
            swap(in[left], in[right]);
            right--;
            left--;
        }
        else
        {
            // left is pivot
            if (left < right)
            {
                swap(in[left], in[left + 1]);
                left--;
            }
        }
    }

    return ret;
}