#include <cstdint>
#include <vector>
#include <future>
#include <thread>
#include <queue>

#include <iostream>

using namespace std;
using namespace AEPKSS::Sort;

namespace AEPKSS::Sort
{
    void quick_sort(vector<size_t> &in);
    size_t quick_sort_parallel(vector<size_t> &in, size_t concurrency);

    struct ParallelQuickSortReturn
    {
        vector<size_t> *in;
        queue<size_t> prefixSumLeft;
        queue<size_t> prefixSumRight;
        size_t countLeft = 0;
        size_t countRight = 0;
    };

    struct ParallelQuickSortReturnComparatorLeft
    {
        bool operator()(ParallelQuickSortReturn &x, ParallelQuickSortReturn &y)
        {
            return x.prefixSumLeft.front() < x.prefixSumLeft.front();
        }
    };

    struct ParallelQuickSortReturnComparatorRight
    {
        bool operator()(ParallelQuickSortReturn &x, ParallelQuickSortReturn &y)
        {
            return x.prefixSumRight.front() < x.prefixSumRight.front();
        }
    };
}

static void sort(vector<size_t> &in, size_t left, size_t right);
static size_t partition(vector<size_t> &in, size_t left, size_t right);

static AEPKSS::Sort::ParallelQuickSortReturn sort_parallel(vector<size_t> &in, size_t pivot);