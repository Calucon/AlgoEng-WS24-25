#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage:" << endl;
        cout << "  ./FileGenerator {filepath} {count}" << endl;
        return EXIT_FAILURE;
    }

    char *strEnd; // just used for long parsing
    auto filePath = argv[1];
    auto intCount = strtol(argv[2], &strEnd, 10);

    cout << "File: " << filePath << " | IntCount: " << intCount << endl;

    // open file handle
    auto fw = AEPKSS::FileWriter(filePath);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int32_t> dist;

    // generate random numbers
    for (auto i = 0; i < intCount; i++)
    {
        auto num = dist(gen);
        fw.write(num);

        if (FILE_GEN_DEBUG)
            cout << "\tW: " << num << endl;
    }

    fw.dispose();
    cout << "Data written to file!" << endl;

    // test to verify everything worked fine
    if (FILE_GEN_DEBUG)
    {
        testFile(filePath, intCount);
        cout << "---" << endl;
        testFile2(filePath, intCount);
    }
}

void testFile(char *filePath, long intCount)
{
    auto fr = AEPKSS::FileReader(filePath);

    optional<uint32_t> tmp;
    uint32_t i = 0;

    while ((tmp = fr.read()).has_value())
    {
        cout << "\tR: " << tmp.value() << endl;
        i++;
    }

    if (i == 0)
    {
        cout << "\tR: vector is empty" << endl;
    }

    fr.dispose();
}

void testFile2(char *filePath, long intCount)
{
    auto fr = AEPKSS::FileReader(filePath);

    for (auto x : fr.read(5))
    {
        cout << "\tR: " << x << endl;
    }

    fr.dispose();
}