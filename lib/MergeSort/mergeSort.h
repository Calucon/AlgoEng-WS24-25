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

    void merge_sort(vector<u_int32_t> &in);
}

static void merge(vector<u_int32_t> &in, u_int32_t left, u_int32_t right, u_int32_t middle);
static void split(vector<u_int32_t> &in, u_int32_t left, u_int32_t right);