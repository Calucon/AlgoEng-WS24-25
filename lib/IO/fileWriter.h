#include <fstream>
#include <vector>

using namespace std;

namespace AEPKSS
{
    class FileWriter
    {
    private:
        const char *filePath;
        bool isDisposed;
        ofstream handle;

    public:
        /**
         * Open file handle
         */
        FileWriter(const char *filePath);

        /**
         * Clean up and close file handle
         */
        ~FileWriter();

        /**
         * Manually free resources, like destructor
         */
        void dispose();

        /**
         * Write buffer to disk
         */
        void flush();

        /**
         * Writes the number to the file.
         */
        void write(uint32_t i, bool flush = false);

        /**
         * Writes the vector to the file.
         */
        void write(vector<uint32_t> data, bool flush = false);
    };
}