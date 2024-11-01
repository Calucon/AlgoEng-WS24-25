#include <algorithm>
#include <iostream>
#include <optional>
#include <chrono>
#include <thread>
#include <future>
#include <queue>

#include "../../lib/IO/fileReader.h"

using namespace std;

#ifndef FILEANALYZER_BUFFER_SIZE
#define FILEANALYZER_BUFFER_SIZE 65536
#endif

#ifndef FILEANALYZER_BUFFER_SIZE_THREADED
#define FILEANALYZER_BUFFER_SIZE_THREADED 1048576
#endif

bool normal(AEPKSS::FileReader &fr, bool isSilent, chrono::steady_clock::time_point t1);
bool threaded(AEPKSS::FileReader &fr, chrono::steady_clock::time_point t1);
void numrange(AEPKSS::FileReader &fr, uint64_t rangeStart, uint64_t rangeEnd);

static bool evalFuture(queue<shared_future<bool>> &futures, chrono::steady_clock::time_point t1);