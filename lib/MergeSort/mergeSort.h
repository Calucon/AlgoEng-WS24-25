#include <vector>

using namespace std;

namespace AEPKSS
{
    void merge_sort(vector<u_int32_t> &in);

    static void merge(vector<u_int32_t> &in, u_int32_t left, u_int32_t right, u_int32_t middle);
    static void split(vector<u_int32_t> &in, u_int32_t left, u_int32_t right);
}