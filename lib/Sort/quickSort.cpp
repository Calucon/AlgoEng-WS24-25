#include "quickSort.h"

void AEPKSS::Sort::quick_sort(vector<size_t> &in)
{
    sort(in, 0, in.size() - 1);
}

void AEPKSS::Sort::sort_parallel(vector<size_t> &in, size_t concurrency)
{
    size_t n = in.size();
    size_t blockSize = n / concurrency;

    if (n < concurrency || concurrency <= 1 || blockSize <= 1)
    {
        AEPKSS::Sort::quick_sort(in);
        return;
    }

    vector<shared_future<vector<size_t>>> cache;
    vector<shared_future<vector<size_t>>> intermediateCache;

    // create job for each processor
    for (auto i = 0; i < concurrency; i++)
    {
        auto start = i * blockSize;
        auto end = start + blockSize;

        vector<size_t> bucket;
        vector<size_t> out;

        // create buckets
        if (i + 1 == concurrency)
            bucket = vector<size_t>(in.begin() + start, in.end());
        else
            bucket = vector<size_t>(in.begin() + start, in.begin() + end);

        const auto func = bind(sort_parallel_lambda, bucket);
        shared_future<vector<size_t>> future = async(launch::async, func);

        intermediateCache.emplace_back(future);
    }
}

static void sort(vector<size_t> &in, size_t left, size_t right)
{
    if (left >= right)
        return;

    auto pIndex = partition(in, left, right);
    sort(in, left, pIndex - 1);
    sort(in, pIndex + 1, right);
}

static size_t partition(vector<size_t> &in, size_t left, size_t right)
{
    auto pivot = in[right];
    size_t swapIndex = left;
    size_t tmp;

    for (auto i = left; i < right; i++)
    {
        if (in[i] > pivot)
            continue;

        swap(in[swapIndex++], in[i]);
    }

    swap(in[swapIndex], in[right]);

    return swapIndex;
}

static vector<size_t> sort_parallel_lambda(vector<size_t> &in)
{
    sort(in, 0, in.size() - 1);
    return in;
}