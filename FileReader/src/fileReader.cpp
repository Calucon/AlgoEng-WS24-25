#include "fileReader.h"

using namespace AEPKSS;

FileReader::FileReader(const char *filePath)
{
    this->filePath = filePath;

    this->handle.open(filePath, ios::binary);
}

FileReader::~FileReader()
{
    this->handle.close();
}

vector<uint32_t> FileReader::read(uint32_t numbers)
{
    vector<uint32_t> v;
    v.reserve(numbers);

    int value;
    streamsize size = sizeof(value);
    for (auto i = 0; i < numbers; i++)
    {
        this->handle.read(reinterpret_cast<char *>(&value), sizeof(value));
        v.push_back(value);

        // abort of EOF is hit
        if (this->handle.eof())
        {
            // shrink vector if there's not enough data
            if (i < numbers)
            {
                v.resize(i);
            }
            break;
        }
    }

    return v;
}