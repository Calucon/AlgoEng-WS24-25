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
    vector<u_int32_t> data;
    optional<u_int32_t> tmp;

    while ((tmp = fr.read()).has_value())
    {
        data.push_back(tmp.value());
    }

    fr.dispose();

    AEPKSS::merge_sort(data);

    fw.write(data);
    fw.dispose();

#pragma endregion

    cout << "Sorting complete!" << endl;
    return EXIT_SUCCESS;
}