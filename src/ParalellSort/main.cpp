#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage:" << endl;
        cout << "  ./ParalellSort {size} {quick|merge}" << endl;
        cout << "      size    -> numbers to generate" << endl;
        cout << "      quick   -> use quicksort" << endl;
        cout << "      merge   -> use mergesort" << endl;
        return EXIT_FAILURE;
    }

    char *strEnd; // just used for long parsing
    const auto size = max(1L, strtol(argv[1], &strEnd, 10));
    const auto algo = string(argv[2]);

    cout << "ParalellSort | Algo: " << algo << ", Size: " << size << endl;

    time_point t1 = chrono::high_resolution_clock::now();
    auto numGenSpeedTest = AEPKSS::Util::NumberGenerator(100UL);
    for (auto i = 0; i < size; i++)
        numGenSpeedTest.next();
    time_point t2 = chrono::high_resolution_clock::now();

    auto numGen = AEPKSS::Util::NumberGenerator(100UL);
    set<uint64_t> s;
    size_t prevSetSize = s.size();

    for (auto i = 0; i < size; i++)
    {
        auto x = numGen.next();
        s.insert(x);

        if (prevSetSize == s.size())
        {
            cout << "ERR: non-distinct number" << endl;
            return EXIT_FAILURE;
        }
        else
            prevSetSize = s.size();

        if (i % 100000 == 0)
            cout << "\tProgress: " << ((double)i / size * 100.0) << "%" << endl;
    }

    cout << "Speed Test: " << ((chrono::duration<double, std::milli>)(t2 - t1)).count() << "ms" << endl;
    cout << "Unique Test: passed!" << endl;
    cout << endl;
    cout << "All done!     -      clean up may take some time" << endl;
    return EXIT_SUCCESS;
}