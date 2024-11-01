#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage:" << endl;
        cout << "  ./FileAnalyzer {filepath} <silent|threaded>" << endl;
        cout << "      silent -> normal analysis, no intermediate console output" << endl;
        cout << "      threaded -> multi-threaded analysis" << endl;
        return EXIT_FAILURE;
    }

    auto filePath = argv[1];
    auto argv2str = string(argv[2]);
    auto isSilent = argc >= 3 && argv2str == "silent";
    auto isThreaded = argc >= 3 && argv2str == "threaded";

    const auto BUFFER_SIZE = isThreaded ? FILEANALYZER_BUFFER_SIZE_THREADED : FILEANALYZER_BUFFER_SIZE;

    auto t1 = chrono::high_resolution_clock::now();
    cout << "Analyzing file: " << filePath << " | Buffer: " << BUFFER_SIZE << ", args: " << (isThreaded ? "threaded" : (isSilent ? "silent" : "")) << endl;

    // open file handle
    auto fr = AEPKSS::FileReader(filePath);

    if (isThreaded)
        threaded(fr, t1);
    else
        normal(fr, isSilent, t1);

    // close handle
    fr.dispose();
}

bool normal(AEPKSS::FileReader &fr, bool isSilent, chrono::steady_clock::time_point t1)
{
    uint32_t prev = 0;
    uint64_t i = 0;
    uint64_t failedAt = 0;

    optional<uint32_t> tmp;
    uint32_t x;
    char buf[11];

    vector<uint32_t> buffer;
    while ((buffer = fr.read(FILEANALYZER_BUFFER_SIZE)).size() > 0)
    {
        for (auto x : buffer)
        {
            if (x < prev && failedAt == 0)
            {
                failedAt = i;
                goto loopExit;
            }

            i++;
            prev = x;

            if (!isSilent)
            {
                sprintf(buf, "%010d", x);
                cout << buf << " [" << i << "]" << endl;
            }
        }
    }

loopExit:
    auto t2 = chrono::high_resolution_clock::now();
    if (failedAt == 0)
    {
        cout << "File analyzed in " << ((chrono::duration<double, std::milli>)(t2 - t1)).count() << "ms! | Everything is sorted correctly!" << endl;
        return true;
    }
    else
    {
        cout << "File analyzed in " << ((chrono::duration<double, std::milli>)(t2 - t1)).count() << "ms! | Failed at: " << failedAt << endl;
        return false;
    }
}

bool threaded(AEPKSS::FileReader &fr, chrono::steady_clock::time_point t1)
{
    unsigned int threadCount = 2 * max(1U, std::thread::hardware_concurrency());
    queue<shared_future<bool>> futures;
    vector<uint32_t> buffer;
    uint32_t last = 0;

    while ((buffer = fr.read(FILEANALYZER_BUFFER_SIZE_THREADED)).size() > 0)
    {
        // check between buffers
        if (last > buffer.front())
        {
            break;
        }
        last = buffer.back();

        if (futures.size() >= threadCount)
        {
            if (!evalFuture(futures, t1))
                return false;
        }

        futures.push(
            async(launch::async, [=]()
                  { return is_sorted(buffer.cbegin(), buffer.cend()); }));
    }

    cout << "Waiting for all futures to complete!" << endl;
    while (!futures.empty())
    {
        if (!evalFuture(futures, t1))
            return false;
    }

    auto t2 = chrono::high_resolution_clock::now();
    cout << "File analyzed in " << ((chrono::duration<double, std::milli>)(t2 - t1)).count() << "ms! | Everything is sorted correctly!" << endl;
    return true;
}

static bool evalFuture(queue<shared_future<bool>> &futures, chrono::steady_clock::time_point t1)
{
    auto f = futures.front();

    f.wait();
    if (!f.get())
    {
        auto t2 = chrono::high_resolution_clock::now();
        cout << "File analyzed in " << ((chrono::duration<double, std::milli>)(t2 - t1)).count() << "ms! | File is NOT sorted!" << endl;
        return false;
    }

    futures.pop();
    return true;
}