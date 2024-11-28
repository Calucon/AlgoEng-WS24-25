#include <cstdint>
#include <algorithm>
#include <vector>
#include <functional>
#include <optional>
#include <queue>
#include <map>

#include <iostream>

#include <future>
#include <thread>

using namespace std;

#ifndef MERGE_SORT_DEBUG
#define MERGE_SORT_DEBUG false
#endif

#ifndef MERGE_SORT_PARALLEL_THRESHOLD
#define MERGE_SORT_PARALLEL_THRESHOLD 10000
#endif

namespace AEPKSS::Sort
{
    enum MergeStrategy
    {
        Classic,
        InMemory,
        Parallel
    };

    struct ParallelMergeData
    {
        optional<shared_future<vector<size_t>>> left = nullopt;
        optional<shared_future<vector<size_t>>> right = nullopt;
    };

    /**
     * This defaults to using `MergeStrategy::Classic`
     */
    void
    merge_sort(vector<size_t> &in);
    void merge_sort(vector<size_t> &in, MergeStrategy strategy);
    size_t merge_sort_parallel(vector<size_t> &in, size_t concurrency);
}

static void split(vector<size_t> &in, size_t left, size_t right, AEPKSS::Sort::MergeStrategy strategy);

static void merge_in_memory(vector<size_t> &in, size_t left, size_t right, size_t middle);
static void merge_classic(vector<size_t> &in, size_t left, size_t right, size_t middle);

/**
 * Merge Sort task per individual processor
 */
static vector<size_t> parallell_merge_per_core(vector<size_t> &in);
/**
 * Merge the result of two processors
 */
static vector<size_t> parallell_merge_block(shared_future<vector<size_t>> &left, optional<shared_future<vector<size_t>>> &right);
/**
 * Merge both inputs and return the result
 */
static vector<size_t> merge_parallel(vector<size_t> &left, vector<size_t> &right);