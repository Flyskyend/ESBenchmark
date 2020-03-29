#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "trace.h"
#include "print.h"

using namespace std;

int CreateTraceFiles(vector<trace> *tracelist, int trace_file_num, double filesize_g)
{
    ofstream tracefile;
    string path = "./TraceFilesDir/TraceFile_";
    string str = "Writing to TraceFile_";

    for (int k = 0; k < trace_file_num; k++)
    {
        cout << path + to_string(k) << endl;

        //gettimeofday(&createTrace_begin, NULL);
        CreateTraceList(tracelist,filesize_g, trace_file_num);
        //gettimeofday(&createTrace_end, NULL);

        //gettimeofday(&sort_begin, NULL);
        cout << "Sorting Trace: ";
        sort(tracelist->begin(), tracelist->end(), TraceListCmp);
        cout << "[100.00%]" << endl;
        //gettimeofday(&sort_end, NULL);

        tracefile.open(path + to_string(k));
        if (tracefile)
        {
            long length = tracelist->size();
            for (int i = 0; i < length; i++)
            {
                for (int j = 0; j < TRACE_HEAD_NUM; j++)
                {
                    tracefile << tracelist->at(i).key[j] << SEPARATOR;
                }
                for (int j = 0; j < TRACE_DATA_NUM; j++)
                {
                    tracefile << tracelist->at(i).data[j] << SEPARATOR;
                }
                tracefile << endl;
                DisplayProgressBar(str + to_string(k), i, length-1);
            }
            tracefile.close();
            cout << endl
                 << "--------------------------" << endl;
        }
        else
            return -1;
    }
    return 0;
}

int ReadOneFromFile(ifstream &ifile, trace *trace)
{
    // if(!ifile)
    //     return -1;
    if (ifile.peek() != EOF)
    {
        for (int i = 0; i < TRACE_HEAD_NUM; i++)
        {
            ifile >> trace->key[i];
        }
        for (int i = 0; i < TRACE_DATA_NUM; i++)
        {
            ifile >> trace->data[i];
        }
		string a;
		getline(ifile,a);
        return 0;
    }
    else
    {
        return 1;
    }
}

int WriteOneToFile(ofstream &ofile, trace *trace)
{
    if (!ofile)
        return -1;
    for (int j = 0; j < TRACE_HEAD_NUM; j++)
	//for (int j = 0; j < 4; j++)
    {
		//cout << "=";
        ofile << trace->key[j] << SEPARATOR;
    }
    for (int j = 0; j < TRACE_DATA_NUM; j++)
	//for (int j = 0; j < 0; j++)
    {
		//cout << "=";
        ofile << trace->data[j] << SEPARATOR;
    }
    ofile << endl;
    return 0;
}