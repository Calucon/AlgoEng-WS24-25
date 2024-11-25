#include "semaphoreTracker.h"

// Initialize static members
AEPKSS::Util::SemaphoreTracker *AEPKSS::Util::SemaphoreTracker::instancePtr = nullptr;
mutex AEPKSS::Util::SemaphoreTracker::mtx;

size_t AEPKSS::Util::SemaphoreTracker::track(binary_semaphore *semaphore)
{
    size_t key = this->startKey++;
    this->kvMap.emplace(key, semaphore);
    return key;
}

binary_semaphore *AEPKSS::Util::SemaphoreTracker::get(size_t key)
{
    return this->kvMap[key];
}

void AEPKSS::Util::SemaphoreTracker::remove(size_t key)
{
    this->kvMap.erase(key);
}