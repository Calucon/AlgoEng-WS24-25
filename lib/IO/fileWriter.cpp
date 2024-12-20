#include "fileWriter.h"

using namespace AEPKSS;

FileWriter::FileWriter(const char *filePath, bool append) : FileWriter(string(filePath), append) {}

FileWriter::FileWriter(const string filePath, bool append)
{
    this->filePath = filePath;
    this->isDisposed = false;

    auto flags = ios::out | ios::binary;
    if (append)
        flags |= ios::app;

    this->handle.open(filePath, flags);
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

    auto size = data.size() * sizeof(uint32_t);
    this->handle.write(reinterpret_cast<char *>(data.data()), size);

    if (flush)
        this->handle.flush();
}