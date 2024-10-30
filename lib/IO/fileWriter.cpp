#include "fileWriter.h"

using namespace AEPKSS;

FileWriter::FileWriter(const char *filePath)
{
    this->filePath = filePath;
    this->isDisposed = false;

    this->handle.open(filePath, ios::binary);
}

FileWriter::~FileWriter()
{
    if (!this->isDisposed)
        this->dispose();
}

void FileWriter::dispose()
{
    this->isDisposed = true;
    this->flush();
    this->handle.close();
}

void FileWriter::flush()
{
    this->handle.flush();
}

void FileWriter::write(uint32_t i, bool flush)
{
    this->handle.write(reinterpret_cast<char *>(&i), sizeof(i));

    if (flush)
        this->handle.flush();
}

void FileWriter::write(vector<uint32_t> data, bool flush)
{
    if (data.size() == 0)
        return;

    auto size = sizeof(data[0]);
    for (auto i : data)
    {
        this->handle.write(reinterpret_cast<char *>(&i), size);
    }

    if (flush)
        this->handle.flush();
}