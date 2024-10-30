#include <algorithm>
#include <vector>
#include <optional>

#include <iostream>

using namespace std;

#ifndef MERGE_SORT_DEBUG
#define MERGE_SORT_DEBUG false
#endif

namespace AEPKSS
{

    void merge_sort(vector<uint32_t> &in);
}

static void merge(vector<uint32_t> &in, uint32_t left, uint32_t right, uint32_t middle);
static void split(vector<uint32_t> &in, uint32_t left, uint32_t right);