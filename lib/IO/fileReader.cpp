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

    // if bytesRead == 0
    if (this->handle.gcount() == 0)
        return nullopt;

    return value;
}

vector<uint32_t> FileReader::read(uint32_t numbers)
{
    // return empty vector if handle is no longer good
    if (!handle.good())
        return vector<uint32_t>();

    vector<uint32_t> v(numbers);
    auto size = sizeof(uint32_t);

    this->handle.read(reinterpret_cast<char *>(v.data()), numbers * size);

    auto numRead = this->handle.gcount() / size;
    if (numRead < numbers)
        v.resize(numRead);

    return v;
}

void FileReader::seek(uint64_t numbers)
{
    if (!handle.good())
        return;

    handle.seekg(numbers * sizeof(uint32_t));
}