#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

namespace AEPKSS
{
    class FileReader
    {
    private:
        const char *filePath;
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
         * Reads {number} integers from the file (if possible)
         */
        vector<uint32_t> read(uint32_t numbers);
    };
}