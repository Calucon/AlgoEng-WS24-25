#include "quickSort.h"

void AEPKSS::Sort::quick_sort(vector<size_t> &in)
{
    sort(in, 0, in.size() - 1);
}

size_t AEPKSS::Sort::quick_sort_parallel(vector<size_t> &in, size_t concurrency)
{
    size_t n = in.size();
    size_t blockSize = n / concurrency;

    if (n < concurrency || concurrency <= 1 || blockSize <= 1)
    {
        quick_sort(in);
        return 1;
    }

    auto pivotIndex = in.size() / 2;
    auto pivot = in[pivotIndex];

    vector<shared_future<ParallelQuickSortReturn>> cache;

    // create job for each processor
    for (auto i = 0; i < concurrency; i++)
    {
        auto start = i * blockSize;
        auto end = start + blockSize;

        vector<size_t> bucket;

        // create buckets
        if (i + 1 == concurrency)
            bucket = vector<size_t>(in.begin() + start, in.end());
        else
            bucket = vector<size_t>(in.begin() + start, in.begin() + end);

        const auto func = bind(sort_parallel, bucket, pivot);
        shared_future<ParallelQuickSortReturn> future = async(launch::async, func);

        cache.emplace_back(future);
    }

    priority_queue<ParallelQuickSortReturn, vector<ParallelQuickSortReturn>, ParallelQuickSortReturnComparatorLeft> queueLeft;
    priority_queue<ParallelQuickSortReturn, vector<ParallelQuickSortReturn>, ParallelQuickSortReturnComparatorRight> queueRight;
    vector<ParallelQuickSortReturn> resRight;
    vector<ParallelQuickSortReturn> resLeft;

    for (auto task : cache)
    {
        auto result = task.get();
        resRight.emplace_back(result);
        resLeft.emplace_back(result);
        queueLeft.push(result);
        queueRight.push(result);
    }

    cout << endl
         << endl;
    for (auto x : in)
    {
        cout << x << endl;
    }
    cout << endl
         << endl;
    cout << "Pivot: " << pivot << endl;

    size_t inOffset = 0;

    while (resLeft.size() > 0)
    {
        // 1 find min prefix sum
        size_t index = 0, minPrefixSum = -1;
        for (auto i = 0; i < resLeft.size(); i++)
        {
            if (resLeft[i].prefixSumLeft.front() < minPrefixSum)
            {
                minPrefixSum = resLeft[i].prefixSumLeft.front();
                index = i;
            }
        }

        auto r = resLeft[index];
        in[inOffset++] = r.in->front();
        r.in->erase(r.in->begin());
        r.prefixSumLeft.pop();
        r.countLeft--;

        if (r.countLeft == 0)
            resLeft.erase(resLeft.begin() + index);
    }

    in[inOffset] = pivot;
    inOffset++;

    /*while (resRight.size() > 0)
    {
        // 1 find min prefix sum
        size_t index = 0, minPrefixSum = -1;
        for (auto i = 0; i < resRight.size(); i++)
        {
            if (resRight[i].prefixSumRight.front() < minPrefixSum)
            {
                minPrefixSum = resRight[i].prefixSumRight.front();
                index = i;
            }
        }

        auto r = resRight[index];
        in[inOffset++] = r.in->at(r.in->end() - r.countRight);
        r.in->erase(r.in->begin());
        r.prefixSumRight.pop();
        r.countRight--;

        if (r.countLeft == 0)
            resRight.erase(resRight.begin() + index);
    }*/

    /*while (!queueRight.empty())
    {
        auto x = queueRight.top();
        auto count = x.countRight;
        if (count == 0)
            continue;

        // 12995105569
        //  9230669516

        for (auto y : x.prefixSumRight)
        {
            cout << y << " - " << (y > pivot) << endl;
        }
        cout << endl
             << endl;

        auto end = x.in->end();
        copy(end - count, end, in.begin() + inOffset);
        inOffset += count;

        queueRight.pop();
    }*/

    cout << endl
         << endl;
    for (auto x : in)
    {
        cout << x << endl;
    }
    cout << endl
         << endl;

    return concurrency;
}

static void sort(vector<size_t> &in, size_t left, size_t right)
{
    if (left >= right)
        return;

    auto pIndex = partition(in, left, right);
    if (pIndex > 0)
        sort(in, left, pIndex - 1);
    sort(in, pIndex + 1, right);
}

static size_t partition(vector<size_t> &in, size_t left, size_t right)
{
    auto pivot = in[right];
    size_t swapIndex = left;

    for (auto i = left; i < right; i++)
    {
        if (in[i] > pivot)
            continue;

        swap(in[swapIndex++], in[i]);
    }

    swap(in[swapIndex], in[right]);

    return swapIndex;
}

static AEPKSS::Sort::ParallelQuickSortReturn sort_parallel(vector<size_t> &in, size_t pivot)
{
    AEPKSS::Sort::ParallelQuickSortReturn ret;

    // preliminary sorting
    AEPKSS::Sort::quick_sort(in);
    ret.in = &in;

    size_t lastSumLeft = 0, lastSumRight = 0;

    for (auto x : in)
    {
        if (x < pivot)
        {
            lastSumLeft += x;
            ret.prefixSumLeft.push(lastSumLeft);
        }
        else if (x > pivot)
        {
            lastSumRight += x;
            ret.prefixSumRight.push(lastSumRight);
        }
    }

    ret.countLeft = ret.prefixSumLeft.size();
    ret.countRight = ret.prefixSumRight.size();

    return ret;
}