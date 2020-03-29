#ifndef ADDER_THREAD
#define ADDER_THREAD

#include <iostream>
#include <vector>
#include <queue>
#include "trace.h"
#include <mutex>

using namespace std;

extern mutex mtx[];

struct thread_data
{
	vector<ifstream> *ifiles;
	vector<int> *readover;
    int *readovernum;
    vector<vector<trace>> *filetop;
	int *needread;
	int *readindex;
	int trace_file_num;
};

void FiletopAdderThread_AddNeed(struct thread_data *arg);
void FiletopAdderThread_TraversalCheck(struct thread_data *arg);

#endif