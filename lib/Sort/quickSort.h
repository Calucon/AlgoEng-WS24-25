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
        vector<size_t> prefixSumLeft;
        vector<size_t> prefixSumRight;
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

static void sort(vector<size_t> &in, size_t left, size_t right);
static size_t partition(vector<size_t> &in, size_t left, size_t right);

static AEPKSS::Sort::ParallelQuickSortReturn sort_parallel(vector<size_t> &in, size_t pivot);
static vector<size_t> position_by_prefixsum(vector<shared_future<AEPKSS::Sort::ParallelQuickSortReturn>> &cache, bool &isLeft);