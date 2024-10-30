#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage:" << endl;
        cout << "  ./MergeSort {inFile} {outFile}" << endl;
        return EXIT_FAILURE;
    }

    auto inFilePath = argv[1];
    auto outFilePath = argv[2];

    cout << "IN: " << inFilePath << " | OUT: " << outFilePath << endl;

    auto fr = AEPKSS::FileReader(inFilePath);
    auto fw = AEPKSS::FileWriter(outFilePath);

#pragma region testing
    auto t1 = chrono::high_resolution_clock::now();

    vector<uint32_t> data;
    optional<uint32_t> tmp;

    while ((tmp = fr.read()).has_value())
    {
        data.push_back(tmp.value());
    }

    fr.dispose();
    auto t2 = chrono::high_resolution_clock::now();
    cout << "\tReading complete after " << ((chrono::duration<double, std::milli>)(t2 - t1)).count() << "ms" << endl;

    AEPKSS::Sort::quick_sort(data);
    auto t3 = chrono::high_resolution_clock::now();
    cout << "\tSorting complete after " << ((chrono::duration<double, std::milli>)(t3 - t2)).count() << "ms" << endl;

    fw.write(data);
    fw.dispose();
    auto t4 = chrono::high_resolution_clock::now();
    cout << "\tWriting complete after " << ((chrono::duration<double, std::milli>)(t4 - t3)).count() << "ms" << endl;

#pragma endregion

    cout << "Sorting complete!" << endl;
    return EXIT_SUCCESS;
}