#include <cstdlib>
#include <chrono>
#include <set>

#include "../../lib/Sort/all.h"
#include "../../lib/Utils/numberGen.h"

using namespace std;
using time_point = std::chrono::_V2::system_clock::time_point;

#ifndef PARALELL_SORT_SEED
#define PARALELL_SORT_SEED 100UL
#endif