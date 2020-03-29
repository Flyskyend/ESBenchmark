#include <iostream>
#include <vector>
#include "trace.h"
#include "print.h"

using namespace std;

void CreateRandTrace(trace *trace, int trace_file_num)
{
	static int time=1;
	int file = time / trace_file_num;
	int t = time % trace_file_num;
	time++;

	trace->key[0] = 1.0 * rand() / 1000000;
	trace->key[1] = file;
	trace->key[2] = t;

    for (int i = 3; i < TRACE_HEAD_NUM; i++)
    {
        trace->key[i] = 1.0 * rand() / 1000000;
    }
    for (int i = 0; i < TRACE_DATA_NUM; i++)
    {
        trace->data[i] = 1.0 * rand() / 1000000;
    }
}

void CreateTraceList(vector<trace> *tracelist,double trace_file_size_g, int trace_file_num)
{
    trace tmp;
    long length = getTracenum(trace_file_size_g);
	tracelist->reserve(length);
    tracelist->clear();

    for (long i = 0; i < length; i++)
    {
        CreateRandTrace(&tmp, trace_file_num);
        tracelist->push_back(tmp);
        DisplayProgressBar("Create Trace List", i, length-1);
        //printf("tracelist_size: %ld(%fGB) trace_num=%d length=%d\n", sizeof(trace) * tracelist->size(), 1.0 * sizeof(trace) * tracelist->size() / 1024 / 1024/1024, tracelist->size(), length);
    }
    cout << endl;
}

bool TraceListCmp(trace a, trace b)
{
    return a.key[0] < b.key[0];
}