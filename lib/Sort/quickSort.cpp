#include "quickSort.h"

void AEPKSS::Sort::quick_sort(vector<uint32_t> &in)
{
    sort(in, 0, in.size() - 1);
}

static void sort(vector<uint32_t> &in, int32_t left, int32_t right)
{
    if (left >= right)
        return;

    auto pIndex = partition(in, left, right);
    sort(in, left, pIndex - 1);
    sort(in, pIndex + 1, right);
}

static uint32_t partition(vector<uint32_t> &in, int32_t left, int32_t right)
{
    auto pivot = in[right];
    auto swapIndex = left - 1;
    uint32_t tmp;

    for (auto i = left; i < right; i++)
    {
        if (in[i] > pivot)
            continue;

        swapIndex++;
        swap(in[swapIndex], in[i]);
    }

    swapIndex++;
    swap(in[swapIndex], in[right]);

    return swapIndex;
}