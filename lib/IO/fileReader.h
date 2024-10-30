#include <fstream>
#include <vector>
#include <iostream>
#include <optional>

using namespace std;

namespace AEPKSS
{
    class FileReader
    {
    private:
        const char *filePath;
        bool isDisposed;
        ifstream handle;

    public:
        /**
         * Open file handle
         */
        FileReader(const char *filePath);

        /**
         * Clean up and close file handle
         */
        ~FileReader();

        /**
         * Manually free resources, like destructor
         */
        void dispose();

        /**
         * Check for EOF
         */
        bool isEOF();

        /**
         * Reads a single integer from the file (if possible)
         */
        optional<uint32_t> read();

        /**
         * Reads {number} integers from the file (if possible)
         */
        vector<uint32_t> read(uint32_t numbers);
    };
}