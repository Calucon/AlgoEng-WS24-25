#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage:" << endl;
        cout << "  ./MergeSort {inFile} {outFile}" << endl;
        return EXIT_FAILURE;
    }

    char *strEnd; // just used for long parsing
    auto inFilePath = argv[1];
    auto outFilePath = argv[2];
    // auto intCount = strtol(argv[2], &strEnd, 10);

    cout << "IN: " << inFilePath << " | OUT: " << outFilePath << endl;

    // TODO: implement

    cout << "Sorting complete!" << endl;
    return EXIT_SUCCESS;
}