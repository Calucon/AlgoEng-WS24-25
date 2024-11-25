#include <cstdlib>
#include <map>
#include <semaphore>

using namespace std;

namespace AEPKSS::Util
{
    /**
     * Simple thread pool implementation
     * @see https://stackoverflow.com/a/32593825
     */
    class SemaphoreTracker
    {
    private:
        // Static pointer to the Singleton instance
        static SemaphoreTracker *instancePtr;
        // Mutex to ensure thread safety
        static mutex mtx;
        // Private Constructor
        SemaphoreTracker() {}

        size_t startKey = 1UL;
        map<size_t, binary_semaphore *> kvMap;

    public:
        // Deleting the copy constructor to prevent copies
        SemaphoreTracker(const SemaphoreTracker &obj) = delete;

        size_t track(binary_semaphore *semaphore);
        binary_semaphore *get(size_t key);
        void remove(size_t key);

        static SemaphoreTracker *getInstance()
        {
            if (instancePtr == nullptr)
            {
                lock_guard<mutex> lock(mtx);
                if (instancePtr == nullptr)
                {
                    instancePtr = new SemaphoreTracker();
                }
            }
            return instancePtr;
        }
    };
}