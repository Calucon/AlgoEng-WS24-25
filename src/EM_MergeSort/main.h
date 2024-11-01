
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <optional>
#include <chrono>
#include <filesystem>

#include "../../lib/IO/fileReader.h"
#include "../../lib/IO/fileWriter.h"
#include "../../lib/Sort/all.h"

using namespace std;

#ifndef TMP_FILE_NAME
#define TMP_FILE_NAME "em_mergesort.tmp"
#endif

/**
 * Reads the Input file, sorts the individual blocks and writes the result in a TMP file
 */
static void sortIntoTmp(const char *inFilePath, const filesystem::path &tmpFilePath, const long &blockSize);

/**
 * Reads the TMP file and merges the blocks into the output file
 */
static void mergeTmpBack(const char *outFilePath, const filesystem::path &tmpFilePath, const long &blockSize);