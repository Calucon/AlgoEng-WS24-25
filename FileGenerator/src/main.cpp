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
    std::ofstream handle(filePath, std::ios::binary);
    // generate random numbers
    for (auto i = 0; i < intCount; i++)
    {
        auto num = std::rand();
        handle.write(reinterpret_cast<const char *>(&num), sizeof(num));
        // std::cout << "\tW: " << num << std::endl;
    }
    // release file handle
    handle.close();

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