#include <cstdint>
#include <vector>

#include <future>
#include <thread>

using namespace std;

namespace AEPKSS::Sort
{
    void quick_sort(vector<size_t> &in);
    void sort_parallel(vector<size_t> &in, size_t concurrency);
}

static void sort(vector<size_t> &in, size_t left, size_t right);
static size_t partition(vector<size_t> &in, size_t left, size_t right);

static vector<size_t> sort_parallel_lambda(vector<size_t> &in);