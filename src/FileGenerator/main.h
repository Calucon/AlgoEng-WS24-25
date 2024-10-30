#include <cstdlib>
#include <iostream>
#include <optional>
#include <random>

#include "../../lib/IO/fileReader.h"
#include "../../lib/IO/fileWriter.h"

using namespace std;

#ifndef FILE_GEN_DEBUG
#define FILE_GEN_DEBUG false
#endif

/**
 * @param argc number of input arguments
 * @param argv input parameters (0 is always the executable path)
 */
int main(int argc, char *argv[]);

/**
 * Used to test the generated file
 */
void testFile(char *filePath, long intCount);
void testFile2(char *filePath, long intCount);