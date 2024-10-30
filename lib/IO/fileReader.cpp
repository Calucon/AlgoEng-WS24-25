#include "fileReader.h"

using namespace AEPKSS;

FileReader::FileReader(const char *filePath)
{
    this->filePath = filePath;
    this->isDisposed = false;

    this->handle.open(filePath, ios::binary);
}

FileReader::~FileReader()
{
    if (!this->isDisposed)
        this->dispose();
}

void FileReader::dispose()
{
    this->isDisposed = true;
    this->handle.close();
}

bool FileReader::isEOF()
{
    return this->handle.eof();
}

optional<uint32_t> FileReader::read()
{
    if (!this->handle.good() || this->handle.eof())
        return nullopt;

    uint32_t value;
    this->handle.read(reinterpret_cast<char *>(&value), sizeof(value));
    return value;
}

vector<uint32_t> FileReader::read(uint32_t numbers)
{
    vector<uint32_t> v;

    // return empty vector if handle is no longer good
    if (!handle.good())
        return v;

    v.reserve(numbers);

    int value;
    streamsize size = sizeof(value);
    for (auto i = 0; i < numbers; i++)
    {
        this->handle.read(reinterpret_cast<char *>(&value), size);
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