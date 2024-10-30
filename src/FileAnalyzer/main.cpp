#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage:" << endl;
        cout << "  ./FileAnalyzer {filepath} <silent>" << endl;
        return EXIT_FAILURE;
    }

    char *strEnd; // just used for long parsing
    auto filePath = argv[1];
    auto isSilent = argc >= 3 && string(argv[2]) == "silent";

    auto t1 = chrono::high_resolution_clock::now();
    cout << "Analyzing file: " << filePath << " | Buffer: " << FILEANALYZER_BUFFER_SIZE << ")" << endl;

    // open file handle
    auto fr = AEPKSS::FileReader(filePath);

    uint32_t prev = 0;
    uint64_t i = 0;
    uint64_t failedAt = 0;
    uint64_t failCount = 0;

    optional<uint32_t> tmp;
    uint32_t x;
    char buf[11];

    vector<uint32_t> buffer;
    while ((buffer = fr.read(FILEANALYZER_BUFFER_SIZE)).size() > 0)
    {
        for (auto x : buffer)
        {
            if (x < prev)
            {
                failCount++;
                if (failedAt == 0)
                    failedAt = i;
            }

            prev = x;
            i++;

            sprintf(buf, "%010d", x);
            if (!isSilent)
                cout << buf << " | (fails: " << failCount << ") [" << i << "]" << endl;
        }
    }

    fr.dispose();

    auto t2 = chrono::high_resolution_clock::now();
    if (failCount == 0)
        cout << "File analyzed in " << ((chrono::duration<double, std::milli>)(t2 - t1)).count() << "ms! | Everything is sorted correctly! " << endl;
    else
        cout << "File analyzed in " << ((chrono::duration<double, std::milli>)(t2 - t1)).count() << "ms! | Fails: " << failCount << ", First: " << failedAt << endl;
}