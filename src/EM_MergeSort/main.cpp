#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "Usage:" << endl;
        cout << "  ./MergeSort {inFile} {outFile} {blockSize}" << endl;
        return EXIT_FAILURE;
    }

    const auto inFilePath = string(argv[1]);
    const auto outFilePath = string(argv[2]);
    const auto tmpFilePath = filesystem::temp_directory_path().append(TMP_FILE_NAME);

    char *strEnd; // just used for long parsing
    const auto blockSize = max(1L, strtol(argv[3], &strEnd, 10));

    cout << "IN: " << inFilePath << " | OUT: " << outFilePath << " | B: " << blockSize << endl;
    auto t1 = chrono::high_resolution_clock::now();

    // sort individual blocks
    auto numbersRead = sortIntoTmp(inFilePath, tmpFilePath, blockSize);
    auto t2 = chrono::high_resolution_clock::now();
    cout << "\tBlock Sorting complete after " << ((chrono::duration<double, std::milli>)(t2 - t1)).count() << "ms" << endl;

    // merge individual blocks
    // mergeTmpBack(outFilePath, tmpFilePath, blockSize);
    queue<MergeJob> mergeQueue = createMergeJobs(blockSize, numbersRead, tmpFilePath.generic_string(), outFilePath);
    MergeJob *lastJob = nullptr;
    cout << "\tQueue Size: " << mergeQueue.size() << endl;
    while (!mergeQueue.empty())
    {
        lastJob = &mergeQueue.front();
        // cout << "Job | id0: " << lastJob->blockId0 << ", id1: " << lastJob->blockId1 << ", seek0: " << lastJob->blockSeek0 << ", seek1: " << lastJob->blockSeek1 << ", OIT: " << lastJob->outFileIsTmp << endl;
        mergeBlocks(*lastJob);
        mergeQueue.pop();
    }
    auto t3 = chrono::high_resolution_clock::now();
    cout << "\tMerging complete after " << ((chrono::duration<double, std::milli>)(t3 - t2)).count() << "ms" << endl;

    // delete or move tmp file
    if (lastJob->outFileIsTmp)
    {
        filesystem::rename(tmpFilePath, filesystem::path(string(outFilePath)));
    }
    else
        filesystem::remove(tmpFilePath);

    cout << "Task complete!" << endl;
    cout << endl;
    return EXIT_SUCCESS;
}

static uint64_t sortIntoTmp(const string inFilePath, const filesystem::path &tmpFilePath, const long &blockSize)
{
    auto frIn = AEPKSS::FileReader(inFilePath);
    auto fwTmp = AEPKSS::FileWriter(tmpFilePath.generic_string().c_str());
    uint64_t numbersRead = 0;

    vector<uint32_t> data;
    while ((data = frIn.read(blockSize)).size() > 0)
    {
        AEPKSS::Sort::intro_sort(data);
        fwTmp.write(data);
        numbersRead += data.size();
    }

    frIn.dispose();
    fwTmp.dispose();
    return numbersRead;
}

static void mergeTmpBack(const string outFilePath, const filesystem::path &tmpFilePath, const long &blockSize)
{
    auto frTmp = AEPKSS::FileReader(tmpFilePath.generic_string().c_str());
    auto fwOut = AEPKSS::FileWriter(outFilePath);

    vector<uint32_t> in0, in1, out;
    vector<uint32_t>::iterator in0iter = in0.end(), in1iter = in1.end();

    do
    {
        // load more elements into in-Buffer if necessary
        if (in0iter == in0.end())
        {
            in0 = frTmp.read(blockSize);
            in0iter = in0.begin();
            if (in0.empty())
                break;
        }
        if (in1iter == in1.end())
        {
            in1 = frTmp.read(blockSize);
            in1iter = in1.begin();
            if (in1.empty())
                break;
        }

        // empty buffer if full
        if (out.size() == blockSize)
        {
            fwOut.write(out);
            out.clear();
            out.reserve(blockSize);
        }

        if (*in0iter < *in1iter)
        {
            out.push_back(*in0iter);
            in0iter++;
        }
        else
        {
            out.push_back(*in1iter);
            in1iter++;
        }
    } while (true);

    // empty remaining vector elements, they must be sorted by now
    for (; in0iter != in0.end(); in0iter++)
    {
        fwOut.write(*in0iter);
    }
    for (; in1iter != in1.end(); in1iter++)
    {
        fwOut.write(*in1iter);
    }

    frTmp.dispose();
    fwOut.dispose();
}

static void mergeBlocks(const MergeJob &job)
{
    if (PRINT_JOB_INFO)
        cout << "\t\tJob_ | id0: " << job.blockId0 << ", id1: " << job.blockId1 << ", seek0: " << job.blockSeek0 << ", seek1: " << job.blockSeek1 << ", OIT: " << job.outFileIsTmp << ", infile: " << job.inFilePath << endl;

    auto frIn0 = AEPKSS::FileReader(job.inFilePath);
    auto frIn1 = AEPKSS::FileReader(job.inFilePath);
    auto fwOut = AEPKSS::FileWriter(job.outFilePath);
    frIn0.seek(job.blockSeek0);
    frIn1.seek(job.blockSeek1);

    const auto bufferSize = job.bufferSize;
    const auto bytesToRead = job.blockInBufferCount * bufferSize;
    uint64_t bytesLeftToRead0 = bytesToRead;
    uint64_t bytesLeftToRead1 = bytesToRead;

    vector<uint32_t> in0, in1, out;
    vector<uint32_t>::iterator in0iter = in0.end(), in1iter = in1.end();
    out.reserve(bufferSize);

    do
    {
        // load more elements into buffer if necessary
        if (in0iter == in0.end())
        {
            if (bytesLeftToRead0 == 0)
                break;

            in0 = frIn0.read(bufferSize);
            in0iter = in0.begin();
            bytesLeftToRead0 -= bufferSize;

            if (in0.empty())
                break;
        }
        if (in1iter == in1.end())
        {
            if (bytesLeftToRead1 == 0)
                break;

            in1 = frIn1.read(bufferSize);
            in1iter = in1.begin();
            bytesLeftToRead1 -= bufferSize;

            if (in1.empty())
                break;
        }

        // empty buffer if full
        if (out.size() == bufferSize)
        {
            fwOut.write(out);
            out.clear();
            out.reserve(bufferSize);
        }

        // compare both buffers
        if (*in0iter < *in1iter)
        {
            out.push_back(*in0iter);
            in0iter++;
        }
        else
        {
            out.push_back(*in1iter);
            in1iter++;
        }
    } while (true);

    // flush buffer
    fwOut.write(out);

    // empty remaining vector elements, they must be sorted by now
    for (; in0iter != in0.end(); in0iter++)
    {
        fwOut.write(*in0iter);
    }
    for (; in1iter != in1.end(); in1iter++)
    {
        fwOut.write(*in1iter);
    }

    frIn0.dispose();
    frIn1.dispose();
    fwOut.dispose();
}

static queue<MergeJob> createMergeJobs(const uint64_t &blockSize, uint64_t &numbersRead, const string tmpFilePath, const string outFilePath)
{
    queue<MergeJob> mq;
    vector<MergeJob> intermediateJobs;
    uint32_t blockId = 0;

    for (uint64_t x = 0; x < numbersRead; x += 2 * blockSize)
    {
        auto b0 = blockId;
        auto b1 = blockId + 1;
        blockId += 2;

        auto job = (MergeJob){
            .blockId0 = b0,
            .blockId1 = b1,
            .blockSeek0 = b0 * blockSize,
            .blockSeek1 = b1 * blockSize,
            .bufferSize = blockSize,
            .blockInBufferCount = 1,
            .inFilePath = tmpFilePath,
            .outFilePath = outFilePath,
            .outFileIsTmp = false};
        intermediateJobs.push_back(job);
    }

    while (intermediateJobs.size() > 1)
    {
        mq.push_range(intermediateJobs);
        vector<MergeJob> jobsToMerge = intermediateJobs;
        intermediateJobs.clear();

        for (size_t i = 0, j = jobsToMerge.size(); i < j; i += 2)
        {
            MergeJob job0 = jobsToMerge[i];
            MergeJob *job1 = ((i + 1) == j) ? nullptr : &jobsToMerge[i + 1];
            intermediateJobs.push_back(mergeJobs(job0, job1));
        }
    }

    mq.push_range(intermediateJobs);
    return mq;
}

static MergeJob mergeJobs(const MergeJob &job0, const MergeJob *optJob1)
{
    if (optJob1 == nullptr)
    {
        return (MergeJob){
            .blockId0 = job0.blockId0,
            .blockId1 = job0.blockId1,
            .blockSeek0 = job0.blockSeek0,
            .blockSeek1 = job0.blockSeek1,
            .bufferSize = job0.bufferSize,
            .blockInBufferCount = job0.blockInBufferCount,
            .inFilePath = job0.inFilePath,
            .outFilePath = job0.outFilePath,
            .outFileIsTmp = !job0.outFileIsTmp};
    }

    auto job1 = *optJob1;
    return (MergeJob){
        .blockId0 = job0.blockId0,
        .blockId1 = job1.blockId0,
        .blockSeek0 = 2 * job0.blockSeek0,
        .blockSeek1 = 2 * job1.blockSeek0,
        .bufferSize = job0.bufferSize,
        .blockInBufferCount = 2 * job0.blockInBufferCount,
        .inFilePath = job0.outFilePath,
        .outFilePath = job0.inFilePath,
        .outFileIsTmp = !job0.outFileIsTmp};
}