#include "mergeSort.h"

void AEPKSS::merge_sort(vector<u_int32_t> &in)
{
    split(in, 0, in.size() - 1);
}

static void AEPKSS::split(vector<u_int32_t> &in, u_int32_t left, u_int32_t right)
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

static void AEPKSS::merge(vector<u_int32_t> &in, u_int32_t left, u_int32_t right, u_int32_t middle)
{
    auto pL = left;
    auto pM = middle + 1;

    u_int32_t x, y;
    optional<u_int32_t> tmp = nullopt;

    for (; pL <= middle; pL++)
    {
        // right side sorted, only left remains
        if (pM >= right)
            break;

        x = in[pL];
        y = in[pM];

        if (y > x)
        {
            // shift data right by 1
            for (auto i = pM; i > pL; i--)
            {
                in[i] = in[i - 1];
            }

            // shift middle pointer right by 1
            pM++;

            // assign value
            in[pL] = y;
        }
    }
}
