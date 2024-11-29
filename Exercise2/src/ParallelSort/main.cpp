#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "Usage:" << endl;
        cout << "  ./ParallelSort {size} {quick|merge} {sta|mta} <concurrency>" << endl;
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
    auto numGen = AEPKSS::Util::NumberGenerator32(PARALELL_SORT_SEED);
    auto inputData = vector<size_t>(SIZE);
    for (auto x = 0; x < SIZE; x++)
    {
        inputData[x] = numGen.next();
    }
    time_point t2 = chrono::high_resolution_clock::now();
    const auto timeGenData = ((chrono::duration<double, std::milli>)(t2 - t1)).count();
    cout << "\tData generation took " << timeGenData << "ms" << endl;

    size_t jobsProcessed = 0;

    if (ALGO == "merge")
    {
        if (TYPE == "mta")
            jobsProcessed = AEPKSS::Sort::merge_sort_parallel(inputData, CONCURRENCY);
        else
            AEPKSS::Sort::merge_sort(inputData, AEPKSS::Sort::Classic);
    }
    else
    {
        if (TYPE == "mta")
        {
            jobsProcessed = AEPKSS::Sort::quick_sort_parallel(inputData, CONCURRENCY);
        }
        else
            AEPKSS::Sort::quick_sort(inputData);
    }

    time_point t3 = chrono::high_resolution_clock::now();
    const auto timeSort = ((chrono::duration<double, std::milli>)(t3 - t2)).count();
    cout << "\tSorting took " << timeSort << "ms" << endl;

    bool isSorted = is_sorted(inputData.cbegin(), inputData.cend());
    time_point t4 = chrono::high_resolution_clock::now();
    const auto timeVerify = ((chrono::duration<double, std::milli>)(t4 - t3)).count();
    cout << "\tValidation took " << timeVerify << "ms" << endl;

    const auto timeTotal = ((chrono::duration<double, std::milli>)(t4 - t1)).count();
    cout << endl;
    cout << "All done!     -      Data is" << (isSorted ? "" : " NOT") << " SORTED!" << endl;
    cout << "              -      Total jobs: " << jobsProcessed << endl;
    cout << "              -      Total execution time: " << timeTotal << "ms" << endl;

    cerr << SIZE << ",";
    cerr << ALGO << ",";
    cerr << TYPE << ",";
    cerr << CONCURRENCY << ",";
    cerr << timeGenData << ",";
    cerr << timeSort << ",";
    cerr << timeVerify << ",";
    cerr << timeTotal << ",";
    cerr << isSorted << endl;
    return EXIT_SUCCESS;
}