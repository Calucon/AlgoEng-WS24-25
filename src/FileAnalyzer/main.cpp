#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage:" << endl;
        cout << "  ./FileAnalyzer {filepath}" << endl;
        return EXIT_FAILURE;
    }

    char *strEnd; // just used for long parsing
    auto filePath = argv[1];

    cout << "Analyzing file: " << filePath << endl;

    // open file handle
    auto fr = AEPKSS::FileReader(filePath);

    uint32_t prev = 0;
    uint64_t i = 0;
    uint64_t failedAt = 0;
    uint64_t failCount = 0;

    optional<u_int32_t> tmp;
    u_int32_t x;
    char buf[11];

    while ((tmp = fr.read()).has_value())
    {
        x = tmp.value();

        if (x < prev)
        {
            failCount++;
            if (failedAt == 0)
                failedAt = i;
        }

        prev = x;
        i++;

        sprintf(buf, "%010d", x);
        cout << buf << " | (fails: " << failCount << ") [" << i << "]" << endl;
    }

    fr.dispose();
    cout << "File analyzed! | (fails: " << failCount << ", first: " << failedAt << ")" << endl;
}