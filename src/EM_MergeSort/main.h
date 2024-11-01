
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <optional>
#include <chrono>
#include <filesystem>
#include <queue>

#include "../../lib/IO/fileReader.h"
#include "../../lib/IO/fileWriter.h"
#include "../../lib/Sort/all.h"

using namespace std;

#ifndef TMP_FILE_NAME
#define TMP_FILE_NAME "em_mergesort.tmp"
#endif

struct MergeJob
{
    /** blockId used to determine the read starting point for the 1st block */
    const uint32_t blockId0;
    /** blockId used to determine the read starting point for the 2nd block */
    const uint32_t blockId1;

    /** how many numbers to seek for block 0 */
    const uint64_t blockSeek0;
    /** how many numbers to seek for block 1 */
    const uint64_t blockSeek1;

    /** default buffer sizes */
    const uint64_t bufferSize;
    /** number of times the block fits into the buffer */
    const uint32_t blockInBufferCount;

    const char *inFilePath;
    const char *outFilePath;
    const bool outFileIsTmp;
};

/**
 * Reads the Input file, sorts the individual blocks and writes the result in a TMP file
 * @return numbers read
 */
static uint64_t sortIntoTmp(const char *inFilePath, const filesystem::path &tmpFilePath, const long &blockSize);

/**
 * Reads the TMP file and merges the blocks into the output file
 */
static void mergeTmpBack(const char *outFilePath, const filesystem::path &tmpFilePath, const long &blockSize);

/**
 * Merges two blocks together
 */
static void mergeBlocks(const MergeJob &job);

/**
 * Create Merge Jobs
 */
static queue<MergeJob> createMergeJobs(const uint64_t &blockSize, uint64_t &numbersRead, const char *tmpFilePath, const char *outFilePath);

/**
 * Combine two MergeJobs
 */
static MergeJob mergeJobs(const MergeJob &job0, const MergeJob *optJob1);