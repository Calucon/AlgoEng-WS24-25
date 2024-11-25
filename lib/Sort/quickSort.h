#include <cstdint>
#include <vector>

using namespace std;

namespace AEPKSS::Sort
{
    void quick_sort(vector<size_t> &in);
}

static void sort(vector<size_t> &in, size_t left, size_t right);
static size_t partition(vector<size_t> &in, size_t left, size_t right);