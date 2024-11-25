#include <cstdint>
#include <algorithm>
#include <vector>
#include <optional>
#include <queue>
#include <map>

#include <iostream>

#include "../Utils/threadPool.h"
#include <semaphore>

using namespace std;

#ifndef MERGE_SORT_DEBUG
#define MERGE_SORT_DEBUG false
#endif

namespace AEPKSS::Sort
{
    enum MergeStrategy
    {
        Classic,
        InMemory,
        Parallel
    };

    /**
     * This defaults to using `MergeStrategy::Classic`
     */
    void merge_sort(vector<size_t> &in);
    void merge_sort(vector<size_t> &in, MergeStrategy strategy);
    void merge_sort_parallel(vector<size_t> &in, size_t concurrency);
}

static void split(vector<size_t> &in, size_t left, size_t right, AEPKSS::Sort::MergeStrategy strategy);
static void split_parallel(vector<size_t> &in, size_t left, size_t right, binary_semaphore *sem, AEPKSS::Util::ThreadPool &pool);

static void merge_in_memory(vector<size_t> &in, size_t left, size_t right, size_t middle);
static void merge_classic(vector<size_t> &in, size_t left, size_t right, size_t middle);