#include "mergeSort.h"

void AEPKSS::merge_sort(vector<uint32_t> &in)
{
    split(in, 0, in.size() - 1);
}

static void split(vector<uint32_t> &in, uint32_t left, uint32_t right)
{
    // end reached, stop splitting
    if (left >= right)
        return;

    // Calculate the midpoint
    int middle = left + ((right - left) / 2);

    if (MERGE_SORT_DEBUG)
        cout << "l: " << left << " | m: " << middle << " | r: " << right << endl;

    // Sort first and second halves
    split(in, left, middle);
    split(in, middle + 1, right);

    // Merge the sorted halves
    merge(in, left, right, middle);
}

static void merge(vector<uint32_t> &in, uint32_t left, uint32_t right, uint32_t middle)
{
    if (MERGE_SORT_DEBUG)
        cout << "\tmerge - l: " << left << " | m: " << middle << " | r: " << right << endl;

    auto pL = left;
    auto pM = middle + 1;

    uint32_t x, y;
    optional<uint32_t> tmp = nullopt;

    for (; pL < pM; pL++)
    {
        // right side sorted, only left remains
        if (pM > right)
            break;

        x = in[pL];
        y = in[pM];

        if (MERGE_SORT_DEBUG)
            cout << "\t\tcmp [" << pL << "," << pM << "] as (" << x << (x < y ? " < " : " > ") << y << ")" << endl;

        if (y < x)
        {
            if (MERGE_SORT_DEBUG)
                cout << "\t\tcmp y > x" << endl;

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
