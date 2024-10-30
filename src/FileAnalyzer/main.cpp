#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage:" << endl;
        cout << "  ./FileAnalyzer {filepath} {count}" << endl;
        return EXIT_FAILURE;
    }

    char *strEnd; // just used for long parsing
    auto filePath = argv[1];
    auto intCount = strtol(argv[2], &strEnd, 10);

    cout << "File: " << filePath << " | Ints to read: " << intCount << endl;

    // open file handle
    auto fr = AEPKSS::FileReader(filePath);
    auto data = fr.read(intCount);

    uint32_t prev = 0;
    uint64_t failedAt = 0;
    uint64_t failCount = 0;
    for (size_t i = 0, j = data.size(); i < j; i++)
    {
        auto x = data[i];

        if (x < prev)
        {
            failCount++;
            if (failedAt == 0)
                failedAt = i;
        }

        prev = x;

        char buf[11];
        sprintf(buf, "%010d", x);
        cout << buf << " | (fails: " << failCount << ")" << endl;
    }

    fr.dispose();
    cout << "File analyzed! | (fails: " << failCount << ", first: " << failedAt << ")" << endl;
}