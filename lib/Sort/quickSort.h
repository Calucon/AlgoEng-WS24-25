#include <vector>

using namespace std;

namespace AEPKSS::Sort
{
    void quick_sort(vector<uint32_t> &in);
}

static void sort(vector<uint32_t> &in, uint32_t left, uint32_t right);
static uint32_t partition(vector<uint32_t> &in, uint32_t left, uint32_t right);