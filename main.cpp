#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "trace.h"
#include "config.h"
#include "print.h"
#include "file.h"
#include "external_sort.h"
#include "time_recorder.h"
//#include "cppkin.h"

using namespace std;

string FileSize(double size)
{
	char filesize[100];
    //unsigned long lsize = TRACE_FILE_SIZE; 
	//cout << size << endl;

	if ((int)size / (1024 * 1024 * 1024ul) > 0)
	{
		snprintf(filesize,sizeof(filesize),"%.2fGB",size / (1024 * 1024 * 1024ul));
	}
	else if ((int)size / (1024 * 1024ul) > 0)
	{
		snprintf(filesize, sizeof(filesize),"%.2fMB",size / (1024 * 1024ul));
	}
	else if ((int)size / 1024ul > 0)
	{
		snprintf(filesize, sizeof(filesize),"%.2fKB",size / 1024ul);
	}
    //cout << filesize << endl;
	return filesize;
}

int main(int argc, char **argv)
{
    struct trace tracetmp;
    vector<trace> tracelist;
    vector<double> file_size_array_g=TRACE_FILE_SIZE_ARRAY_G;
    vector<int> file_num_array=TRACE_FILE_NUM_ARRAY;
	TimeRecorder TR;
	ofstream resultfile;
	resultfile.open("./ESBenchmarkResult");

	PrintandWrite(resultfile, "\n============================ Benchmark Start ============================\n");
	PrintandWrite(resultfile, "----------------------------------------\n");
	PrintandWrite(resultfile, "TRACE_HEAD_NUM = %d\n",TRACE_HEAD_NUM);
	PrintandWrite(resultfile, "TRACE_DATA_NUM = %d\n",TRACE_DATA_NUM);
	PrintandWrite(resultfile, "TRACE_SORT_ITEM_INDEX = %d\n",TRACE_SORT_ITEM_INDEX);
	PrintandWrite(resultfile, "FILETOP_BUFFER_SIZE = %d\n",FILETOP_BUFFER_SIZE);
	PrintandWrite(resultfile, "MULTI_THREAD_ADDER = %s\n",MULTI_THREAD_ADDER?"Yes":"No");
	PrintandWrite(resultfile, "CREATE_TRACE_FILE = %s\n",CREATE_TRACE_FILE?"Yes":"No");
	PrintandWrite(resultfile, "USE_LOSER_TREE = %s\n",USE_LOSER_TREE?"Yes":"No");
	PrintandWrite(resultfile, "SEPARATOR = \"%s\"\n",SEPARATOR);
	PrintandWrite(resultfile, "----------------------------------------\n");

    //struct timeval LTSort_begin,LTSort_end,DSort_begin, DSort_end;
    //gettimeofday(&t_begin, NULL);
	TR.AddTRBegin("Benchmark_Total_Time");
    for(auto filesize_g : file_size_array_g)
    {
        //gettimeofday(&createFiles_begin, NULL);
		TR.AddTRBegin("Files_Creating_Time");
        if (CREATE_TRACE_FILE)
        {
            if (CreateTraceFiles(&tracelist,file_num_array.front(),filesize_g))
                cout << "Create Trace Files Fail !!" << endl;
        }
        //gettimeofday(&createFiles_end, NULL);
		TR.AddTREnd("Files_Creating_Time");
		PrintandWrite(resultfile, "======================================================================\n");
		PrintandWrite(resultfile, "Files_Creating_Time : %.2fs\n", TR.GetTRTimeuse("Files_Creating_Time"));

        for(auto filenum : file_num_array)
        {
            if(MULTI_THREAD_ADDER)
            {
                if (USE_LOSER_TREE)
                {
                    //gettimeofday(&LTSort_begin, NULL);
					TR.AddTRBegin("LoserTree_MT_Sorting_Time");
                    if(SortFiles_LoserTree_MT(filenum, filesize_g))
                        cout << "External Sorting Error !!" << endl;
                    //gettimeofday(&LTSort_end, NULL);
					TR.AddTREnd("LoserTree_MT_Sorting_Time");
                }
                if (USE_DEFAULT)
                {
                    //gettimeofday(&DSort_begin, NULL);
					TR.AddTRBegin("Default_MT_Sorting_Time");
                    if (SortFiles_Default_MT(filenum, filesize_g))
                        cout << "External Sorting Error !!" << endl;
                    //gettimeofday(&DSort_end, NULL);
					TR.AddTREnd("Default_MT_Sorting_Time");
                }
            }
			if (USE_LOSER_TREE)
			{
				//gettimeofday(&LTSort_begin, NULL);
				TR.AddTRBegin("LoserTree_ST_Sorting_Time");
				if(SortFiles_LoserTree_ST(filenum, filesize_g))
					cout << "External Sorting Error !!" << endl;
				//gettimeofday(&LTSort_end, NULL);
				TR.AddTREnd("LoserTree_ST_Sorting_Time");
			}
			if (USE_DEFAULT)
			{
				//gettimeofday(&DSort_begin, NULL);
				TR.AddTRBegin("Default_ST_Sorting_Time");
				if (SortFiles_Default_ST(filenum,filesize_g))
					cout << "External Sorting Error !!" << endl;
				//gettimeofday(&DSort_end, NULL);
				TR.AddTREnd("Default_ST_Sorting_Time");
			}

			long size = toGsize(filesize_g);
			PrintandWrite(resultfile, "###################################################\n");
			PrintandWrite(resultfile, ">>>>>>> Result <<<<<<<\n");
			PrintandWrite(resultfile, "TRACE_FILE_SIZE = %d(%s)\n",size,FileSize(toGsize(filesize_g)).c_str());
			PrintandWrite(resultfile, "TRACE_FILE_NUM = %d\n", filenum);
			PrintandWrite(resultfile, "Trace Size: %d(%s) =%d*%d+%d*%d\n", sizeof(tracetmp), FileSize(sizeof(trace)).c_str(), TRACE_HEAD_NUM, sizeof(double), TRACE_DATA_NUM, sizeof(double));
			PrintandWrite(resultfile, "Tracelist Size: %ld(%s) \nTrace_Num: %d\n",tracelist.size()*sizeof(trace), FileSize(tracelist.size() * sizeof(trace)).c_str(), getTracenum(filesize_g));
			if (MULTI_THREAD_ADDER)
			{
				PrintandWrite(resultfile, "LoserTree_MT_Sorting_Time : %.2fs\n", TR.GetTRTimeuse("LoserTree_MT_Sorting_Time"));
				PrintandWrite(resultfile, "Default_MT_Sorting_Time : %.2fs\n", TR.GetTRTimeuse("Default_MT_Sorting_Time"));
			}
			PrintandWrite(resultfile, "LoserTree_ST_Sorting_Time : %.2fs\n", TR.GetTRTimeuse("LoserTree_ST_Sorting_Time"));
			PrintandWrite(resultfile, "Default_ST_Sorting_Time : %.2fs\n", TR.GetTRTimeuse("Default_ST_Sorting_Time"));
        }

    }

    //gettimeofday(&t_end, NULL);
	TR.AddTREnd("Benchmark_Total_Time");
	PrintandWrite(resultfile, "============================ Benchmark End ============================\n\n");
	PrintandWrite(resultfile, "Benchmark_Total_Time : %.2fs\n", TR.GetTRTimeuse("Benchmark_Total_Time"));

	resultfile.close();

	return 0;
}