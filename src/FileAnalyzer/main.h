#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <optional>
#include <chrono>
#include <thread>
#include <future>
#include <queue>

#include "../../lib/IO/fileReader.h"

using namespace std;
using time_point = std::chrono::_V2::system_clock::time_point;

#ifndef FILEANALYZER_BUFFER_SIZE
#define FILEANALYZER_BUFFER_SIZE 65536
#endif

#ifndef FILEANALYZER_BUFFER_SIZE_THREADED
#define FILEANALYZER_BUFFER_SIZE_THREADED 1048576
#endif

bool normal(AEPKSS::FileReader &fr, bool isSilent, time_point t1);
bool threaded(AEPKSS::FileReader &fr, time_point t1);
void numrange(AEPKSS::FileReader &fr, uint64_t rangeStart, uint64_t rangeEnd);

static bool evalFuture(queue<shared_future<bool>> &futures, time_point t1);