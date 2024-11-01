#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "Usage:" << endl;
        cout << "  ./MergeSort {inFile} {outFile} {blockSize}" << endl;
        return EXIT_FAILURE;
    }

    const auto inFilePath = argv[1];
    const auto outFilePath = argv[2];
    const auto tmpFilePath = filesystem::temp_directory_path().append(TMP_FILE_NAME);

    char *strEnd; // just used for long parsing
    const auto blockSize = max(1L, strtol(argv[3], &strEnd, 10));

    cout << "IN: " << inFilePath << " | OUT: " << outFilePath << " | B: " << blockSize << endl;
    auto t1 = chrono::high_resolution_clock::now();

    // sort individual blocks
    sortIntoTmp(inFilePath, tmpFilePath, blockSize);
    auto t2 = chrono::high_resolution_clock::now();
    cout << "\tBlock Sorting complete after " << ((chrono::duration<double, std::milli>)(t2 - t1)).count() << "ms" << endl;

    // merge individual blocks
    mergeTmpBack(outFilePath, tmpFilePath, blockSize);
    auto t3 = chrono::high_resolution_clock::now();
    cout << "\tMerging complete after " << ((chrono::duration<double, std::milli>)(t3 - t2)).count() << "ms" << endl;

    /// delete tmp file
    filesystem::remove(tmpFilePath);
    cout << "Task complete!" << endl;
    return EXIT_SUCCESS;
}

static void sortIntoTmp(const char *inFilePath, const filesystem::path &tmpFilePath, const long &blockSize)
{
    auto frIn = AEPKSS::FileReader(inFilePath);
    auto fwTmp = AEPKSS::FileWriter(tmpFilePath.generic_string().c_str());

    vector<uint32_t> data;
    data.reserve(blockSize);
    while ((data = frIn.read(blockSize)).size() > 0)
    {
        AEPKSS::Sort::intro_sort(data);
        fwTmp.write(data);
    }

    frIn.dispose();
    fwTmp.dispose();
}

static void mergeTmpBack(const char *outFilePath, const filesystem::path &tmpFilePath, const long &blockSize)
{
    auto frTmp = AEPKSS::FileReader(tmpFilePath.generic_string().c_str());
    auto fwOut = AEPKSS::FileWriter(outFilePath);

    vector<uint32_t> in0, in1, out;
    vector<uint32_t>::iterator in0iter = in0.end(), in1iter = in1.end();

    do
    {
        // load more elements into in-Buffer if necessary
        if (in0iter == in0.end())
        {
            in0 = frTmp.read(blockSize);
            in0iter = in0.begin();
            if (in0.empty())
                break;
        }
        if (in1iter == in1.end())
        {
            in1 = frTmp.read(blockSize);
            in1iter = in1.begin();
            if (in1.empty())
                break;
        }

        // empty buffer if full
        if (out.size() == blockSize)
        {
            fwOut.write(out);
            out.clear();
            out.reserve(blockSize);
        }

        if (*in0iter < *in1iter)
        {
            out.push_back(*in0iter);
            in0iter++;
        }
        else
        {
            out.push_back(*in1iter);
            in1iter++;
        }
    } while (true);

    // empty remaining vector elements, they must be sorted by now
    for (; in0iter != in0.end(); in0iter++)
    {
        fwOut.write(*in0iter);
    }
    for (; in1iter != in1.end(); in1iter++)
    {
        fwOut.write(*in1iter);
    }

    frTmp.dispose();
    fwOut.dispose();
}