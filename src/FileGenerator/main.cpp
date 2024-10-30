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

    // set rand seed()
    srand((unsigned int)time(NULL));

    // generate random numbers
    for (auto i = 0; i < intCount; i++)
    {
        auto num = rand();
        fw.write(num);
        // cout << "\tW: " << num << endl;
    }

    fw.dispose();
    cout << "Data written to file!" << endl;

    // test to verify everything worked fine
    testFile(filePath, intCount);
}

void testFile(char *filePath, long intCount)
{
    auto fr = AEPKSS::FileReader(filePath);

    auto data = fr.read(3);
    for (auto element : data)
    {
        cout << "\tR: " << element << endl;
    }
    if (data.size() == 0)
    {
        cout << "\tR: vector is empty" << endl;
    }
}