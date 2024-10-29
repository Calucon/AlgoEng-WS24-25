#include "mergeSort.h"

vector<u_int32_t> AEPKSS::merge_sort(vector<u_int32_t> in)
{
    split(in, 0, in.size() - 1);
}

// annonymous namespace to not expose functions
namespace
{
    void split(vector<u_int32_t> in, u_int32_t left, u_int32_t right)
    {
        // end reached, stop splitting
        if (left >= right)
            return;

        // Calculate the midpoint
        int middle = left + ((right - left) / 2);

        // Sort first and second halves
        split(in, left, middle);
        split(in, middle + 1, right);

        // Merge the sorted halves
        merge(in, left, middle, right);
    }

    void merge(vector<u_int32_t> in, u_int32_t left, u_int32_t right, u_int32_t middle)
    {
        // TODO: implement
    }
}