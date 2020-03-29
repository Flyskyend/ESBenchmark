#ifndef TRACE
#define TRACE

#include <iostream>
#include <vector>
#include "config.h"

struct trace
{
    double key[TRACE_HEAD_NUM];
    double data[TRACE_DATA_NUM];
};

void CreateRandTrace(trace *trace, int trace_file_num);
void CreateTraceList(std::vector<trace> *tracelist,double filesize_g, int trace_file_num);
bool TraceListCmp(trace a, trace b);

#endif