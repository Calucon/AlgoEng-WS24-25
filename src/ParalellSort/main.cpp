#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "Usage:" << endl;
        cout << "  ./ParalellSort {size} {quick|merge} {sta|mta} <concurrency>" << endl;
        cout << "      size         -> numbers to generate" << endl;
        cout << endl;
        cout << "      quick        -> use quicksort" << endl;
        cout << "      merge        -> use mergesort" << endl;
        cout << endl;
        cout << "      sta          -> Single Threaded" << endl;
        cout << "      mta          -> Multi Threaded" << endl;
        cout << endl;
        cout << "      concurrency  -> number of threads to use" << endl;
        return EXIT_FAILURE;
    }

    char *strEnd; // just used for long parsing
    const auto SIZE = max(1L, stol(string(argv[1])));
    const auto ALGO = string(argv[2]);
    const auto TYPE = string(argv[3]);
    const auto CONCURRENCY = argc >= 5 ? max(1L, stol(string(argv[4]))) : thread::hardware_concurrency();

    if (ALGO != "quick" && ALGO != "merge")
    {
        cerr << "Invalid Algorithm! {quick|merge}" << endl;
        return EXIT_FAILURE;
    }

    if (TYPE != "mta" && TYPE != "sta")
    {
        cerr << "Invalid Threading Type! {sta|mta}" << endl;
        return EXIT_FAILURE;
    }

    cout << "ParalellSort | Algo: " << ALGO << ", Size: " << SIZE << ", Type: " << TYPE << ", Concurrency: " << CONCURRENCY << endl;

    time_point t1 = chrono::high_resolution_clock::now();
    auto numGen = AEPKSS::Util::NumberGenerator(PARALELL_SORT_SEED);
    auto inputData = vector<size_t>(SIZE);
    for (auto x = 0; x < SIZE; x++)
    {
        inputData[x] = numGen.next();
    }
    time_point t2 = chrono::high_resolution_clock::now();
    cout << "\tData generation took " << ((chrono::duration<double, std::milli>)(t2 - t1)).count() << "ms" << endl;

    if (ALGO == "merge")
    {
        if (TYPE == "mta")
            AEPKSS::Sort::merge_sort(inputData, AEPKSS::Sort::Parallel);
        else
            AEPKSS::Sort::merge_sort(inputData, AEPKSS::Sort::Classic);
    }
    else
    {
        if (TYPE == "mta")
        {
            cerr << "Quick Sort not supported (yet)!" << endl;
            return EXIT_FAILURE;
        }
        else
            AEPKSS::Sort::quick_sort(inputData);
    }

    time_point t3 = chrono::high_resolution_clock::now();
    cout << "\tSorting took " << ((chrono::duration<double, std::milli>)(t3 - t2)).count() << "ms" << endl;

    bool isSorted = is_sorted(inputData.cbegin(), inputData.cend());
    time_point t4 = chrono::high_resolution_clock::now();
    cout << "\tValidation took " << ((chrono::duration<double, std::milli>)(t4 - t3)).count() << "ms" << endl;

    cout << endl;
    cout << "All done!     -      Data is" << (isSorted ? "" : " NOT") << " SORTED!" << endl;
    cout << "              -      Total execution time: " << ((chrono::duration<double, std::milli>)(t4 - t1)).count() << "ms" << endl;
    return EXIT_SUCCESS;
}