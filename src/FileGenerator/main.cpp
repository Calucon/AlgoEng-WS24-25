#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "  ./FileGenerator {filepath} {count}" << std::endl;
        return EXIT_FAILURE;
    }

    char *strEnd; // just used for long parsing
    auto filePath = argv[1];
    auto intCount = std::strtol(argv[2], &strEnd, 10);

    std::cout << "File: " << filePath << " | IntCount: " << intCount << std::endl;

    // open file handle
    auto fw = AEPKSS::FileWriter(filePath);
    // generate random numbers
    for (auto i = 0; i < intCount; i++)
    {
        auto num = std::rand();
        fw.write(num);
        // std::cout << "\tW: " << num << std::endl;
    }

    std::cout << "Data written to file!" << std::endl;

    // test to verify everything worked fine
    testFile(filePath, intCount);
}

void testFile(char *filePath, long intCount)
{
    auto fr = AEPKSS::FileReader(filePath);

    auto data = fr.read(3);
    for (auto element : data)
    {
        std::cout << "\tR: " << element << std::endl;
    }
    if (data.size() == 0)
    {
        std::cout << "\tR: vector is empty" << std::endl;
    }
}