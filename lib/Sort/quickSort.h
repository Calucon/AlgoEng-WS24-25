#include <cstdint>
#include <vector>
#include <future>
#include <thread>
#include <queue>

#include <iostream>

using namespace std;
using namespace AEPKSS::Sort;

#ifndef QUICK_SORT_THRESHOLD
#define QUICK_SORT_THRESHOLD 1000
#endif

namespace AEPKSS::Sort
{
    void quick_sort(vector<size_t> &in);
    size_t quick_sort_parallel(vector<size_t> &in, size_t concurrency);

    struct ParallelQuickSortReturn
    {
        vector<size_t> *in;
        size_t countLeft = 0;
        size_t countRight = 0;
    };

    struct ParallelQuickSortPairComparator
    {
        bool operator()(pair<size_t, size_t> &x, pair<size_t, size_t> &y)
        {
            return x.first > y.first;
        }
    };
}

static void sort(vector<size_t> &in, long long left, long long right);
static size_t partition(vector<size_t> &in, long long left, long long right);

static size_t partition_parallel(vector<size_t> &in, size_t concurrency);
static AEPKSS::Sort::ParallelQuickSortReturn sort_parallel(vector<size_t> &in, size_t pivot);