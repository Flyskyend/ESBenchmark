#include <iostream>
#include <vector>
#include <string>
#include <stdarg.h>
#include <fstream>
#include "trace.h"

using namespace std;

void PrintTrace(trace *trace, int type)
{
    //for (int i = 0; i < TRACE_HEAD_NUM && type < 2; i++)
    for (int i = 0; i < 1 && type < 2; i++)
    {
        printf("trace_key[%d]= %f\n", i, trace->key[i]);
    }
    for (int i = 0; i < TRACE_DATA_NUM && type > 0; i++)
    {
        printf("trace_data[%d]= %f\n", i, trace->data[i]);
    }
}

void PrintTraceList(vector<trace> *tracelist)
{
    for (int i = 0; i < tracelist->size(); i++)
    {
        cout << "trace" << i << ":" << tracelist->at(i).key[0] << endl;
    }
}

void DisplayProgressBar(string cont, long now, long total)
{
    string bar = ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    // static int time=0;
    // int freq = 1000;
	double percent = 1.0 * now / total;
	int barlen = bar.size() * percent;

    // if(time++%freq==0||now==total-1)
	printf("%s:  %-40s[%.1f%%]\r",cont.c_str(), bar.substr(0, barlen).c_str(), now<total?percent * 100:100);
}

void PrintandWrite(ofstream &resultfile, const char *format, ...)
{
	char buf[1000];

	va_list ap;
	va_start(ap, format);
	vsprintf(buf, format, ap);

	resultfile << buf;
	cout << buf;
	va_end(ap);
}