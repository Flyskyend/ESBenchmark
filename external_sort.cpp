#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include "adder_thread.h"
#include "trace.h"
#include "file.h"
#include "print.h"
#include "loser_tree.h"

using namespace std;

int CheckFiles(vector<ifstream> &ifiles, int trace_file_num)
{
	string path = "./TraceFilesDir/TraceFile_";

	for (int i = 0; i < trace_file_num; i++)
	{
		//cout << path + to_string(i) << endl;
		ifiles[i].open(path + to_string(i));
		if (!ifiles[i])
		{
			cout << "open file: " << path + to_string(i) << " fail !!" << endl;
			return -1;
		}
	}
	return 0;
}

int LoadBuffer(int trace_file_num,int filetop_buffer_size,vector<int> &readover, int &readovernum, vector<ifstream> &ifiles, vector<vector<trace>> &filetop)
{
	for (int j = 0; j < trace_file_num && !readover[j]; j++)
	{
		for (int k = 0; k < filetop_buffer_size; k++)
		{
			trace tmp;
			int ret = ReadOneFromFile(ifiles[j], &tmp);
			if (ret == 1)
			{
				cout << "File " << j << " Read Over." << endl;
				readover[j] = 1;
				readovernum++;
				ifiles[j].close();
				break;
			}
			else if (ret == -1)
			{
				cout << "Read One From File: " << j << " Error !!" << endl;
				return -1;
			}
			else
			{
				//cout << "File " << j << ": ";
				//PrintTrace(&tmp, 0);
				filetop[j].push_back(tmp);
			}
		}
		//cout<<"file"<<j<<":"<<filetop.at(j).size()<<endl;
	}
	return 0;
}

int FindMin(vector<vector<trace>> filetop, vector<int> &sortover, int trace_file_num)
{
	double min = -1;
	int index;

	for (int i = 0; i < trace_file_num; i++)
	{
		//cout << "-";

		if (!sortover[i] && (min < 0 || min > filetop[i].front().key[TRACE_SORT_ITEM_INDEX])) //Modify
		{
			//cout<<i<<" ";
			min = filetop[i].front().key[TRACE_SORT_ITEM_INDEX];
			index = i;
		}
	}
	//cout<<endl;
	return index;
}

int SortFiles_LoserTree_MT(int trace_file_num, double trace_file_size_g)
{
	vector<ifstream> ifiles(trace_file_num);
	ofstream ofile;
	vector<vector<trace>> filetop(trace_file_num);

	vector<int> readover(trace_file_num);
	vector<int> sortover(trace_file_num);
	int readovernum = 0;
	int sortovernum = 0;

	long time = 0;
	long total = getTracenum(trace_file_size_g)*trace_file_num;

	cout<< "\n====================================" << endl;
	cout<< "-Loser Tree Sorting (Multi-Thread)-" << endl;
	cout<< "====================================" << endl;

	ofile.open("./ESTraceResult");

	if (CheckFiles(ifiles, trace_file_num))
		return -1;
	if (LoadBuffer(trace_file_num,FILETOP_BUFFER_SIZE,readover, readovernum, ifiles, filetop))
		return -1;

	int min = -1;

	LoserTree LTree((int)trace_file_num, &filetop);

	struct thread_data td;
	td.filetop = &filetop;
	td.ifiles = &ifiles;
	td.readover = &readover;
	td.readovernum = &readovernum;
	td.trace_file_num = trace_file_num;
	cout<< "-Creating Filetop Adder Thread..." << endl;
	thread adderthread(FiletopAdderThread_TraversalCheck, &td);

	while (sortovernum < trace_file_num)
	{
		min = LTree.GetWinner();
		if (WriteOneToFile(ofile, &filetop[min].front()))
		{
			cout << "Write One To File(min): " << min << " Error !!" << endl;
			return -1;
		}
		//mtx[min].lock();
		filetop[min].erase(filetop[min].begin());
		//mtx[min].unlock();
		LTree.UpdateTree();

		if (readover[min] && filetop[min].empty())
		{
			sortover[min] = 1;
			sortovernum++;
			cout << "-------------File " << min << " Sort Over. sortovernum=" << sortovernum << endl;
		}

		DisplayProgressBar("External Sorting", ++time, total);
	}
	adderthread.join();
	ofile.close();
	cout << "\n-External Sorting Completed." << endl;
	return 0;
}

int SortFiles_Default_MT(int trace_file_num, double trace_file_size_g)
{
	vector<ifstream> ifiles(trace_file_num);
	ofstream ofile;
	vector<vector<trace>> filetop(trace_file_num);

	vector<int> readover(trace_file_num);
	vector<int> sortover(trace_file_num);
	int readovernum = 0;
	int sortovernum = 0;

	long time = 0;
	long total = getTracenum(trace_file_size_g)*trace_file_num;

	cout<< "\n====================================" << endl;
	cout<< "-Default Sorting (Multi-Thread)-" << endl;
	cout<< "====================================" << endl;

	ofile.open("./ESTraceResult");

	if (CheckFiles(ifiles, trace_file_num))
		return -1;
	if (LoadBuffer(trace_file_num,FILETOP_BUFFER_SIZE,readover, readovernum, ifiles, filetop))
		return -1;

	int min = -1;

	struct thread_data td;
	td.filetop = &filetop;
	td.ifiles = &ifiles;
	td.readover = &readover;
	td.readovernum = &readovernum;
	td.trace_file_num = trace_file_num;
	cout << "-Creating Filetop Adder Thread..." << endl;
	thread adderthread(FiletopAdderThread_TraversalCheck, &td);

	while (sortovernum < trace_file_num)
	{
		min = FindMin(filetop, sortover, trace_file_num);
		trace tmp = filetop[min].front();
		//mtx[min].lock();
		filetop[min].erase(filetop[min].begin());
		//mtx[min].unlock();

		if (WriteOneToFile(ofile, &tmp))
		{
			cout << "Write One To File(min): " << min << " Error !!" << endl;
			return -1;
		}
		if (readover[min] && filetop[min].empty())
		{
			sortover[min] = 1;
			sortovernum++;
			cout << "-------------File " << min << " Sort Over. sortovernum=" << sortovernum << endl;
		}

		DisplayProgressBar("External Sorting", ++time, total);
	}
	adderthread.join();
	ofile.close();
	cout << "\nEnternal Sorting Completed." << endl;
	return 0;
}

int SortFiles_LoserTree_ST(int trace_file_num, double trace_file_size_g)
{
	vector<ifstream> ifiles(trace_file_num);
	ofstream ofile;
	vector<vector<trace>> filetop(trace_file_num);

	vector<int> readover(trace_file_num);
	vector<int> sortover(trace_file_num);
	int readovernum = 0;
	int sortovernum = 0;

	long time = 0;
	long tracenum = getTracenum(trace_file_size_g);
	long total = getTracenum(trace_file_size_g) * trace_file_num;

	cout<< "\n====================================" << endl;
	cout<< "-Loser Tree Sorting (Single-Thread)-" << endl;
	cout<< "====================================" << endl;

	ofile.open("./ESTraceResult");

	if (CheckFiles(ifiles, trace_file_num))
		return -1;
	if (LoadBuffer(trace_file_num,1,readover, readovernum, ifiles, filetop))
		return -1;

	int min = -1;

	LoserTree LTree((int)trace_file_num, &filetop);

	while (sortovernum < trace_file_num)
	{
		min = LTree.GetWinner();
		if (WriteOneToFile(ofile, &filetop[min].front()))
		{
			cout << "Write One To File(min): " << min << " Error !!" << endl;
			return -1;
		}
		filetop[min].erase(filetop[min].begin());
		LTree.UpdateTree();

		trace tmp;
		int ret = ReadOneFromFile(ifiles[min], &tmp);
		if (ret == 1)
		{
			readover[min] = 1;
			readovernum += 1;
			ifiles[min].close();
			cout << " -----------------------------------------File " << min << " Read Over. readovernum=" << readovernum << endl;
		}
		else if (ret == -1)
		{
			cout << "Read One From File: " << min << " Error !!" << endl;
			return -1;
		}
		else
		{
			filetop[min].push_back(tmp);
		}

		if (readover[min] && filetop[min].empty())
		{
			sortover[min] = 1;
			sortovernum++;
			cout << "-------------File " << min << " Sort Over. sortovernum=" << sortovernum << endl;
		}

		DisplayProgressBar("External Sorting", ++time, total);
	}

	ofile.close();
	cout << "\n-External Sorting Completed." << endl;
	return 0;
}

int SortFiles_Default_ST(int trace_file_num, double trace_file_size_g)
{
	vector<ifstream> ifiles(trace_file_num);
	ofstream ofile;
	vector<vector<trace>> filetop(trace_file_num);

	vector<int> readover(trace_file_num);
	vector<int> sortover(trace_file_num);
	int readovernum = 0;
	int sortovernum = 0;

	long time = 0;
	long total = getTracenum(trace_file_size_g)*trace_file_num;

	cout<< "\n====================================" << endl;
	cout<< "-Default Sorting (Single-Thread)-" << endl;
	cout<< "====================================" << endl;

	ofile.open("./ESTraceResult");

	if (CheckFiles(ifiles, trace_file_num))
		return -1;
	if (LoadBuffer(trace_file_num,1,readover, readovernum, ifiles, filetop))
		return -1;

	int min = -1;

	while (sortovernum < trace_file_num)
	{
		min = FindMin(filetop, sortover, trace_file_num);
		trace tmp = filetop[min].front();
		filetop[min].erase(filetop[min].begin());

		if (WriteOneToFile(ofile, &tmp))
		{
			cout << "Write One To File(min): " << min << " Error !!" << endl;
			return -1;
		}

		int ret = ReadOneFromFile(ifiles[min], &tmp);
		if (ret == 1)
		{
			readover[min] = 1;
			readovernum += 1;
			ifiles[min].close();
			cout << " -----------------------------------------File " << min << " Read Over. readovernum=" << readovernum << endl;
		}
		else if (ret == -1)
		{
			cout << "Read One From File: " << min << " Error !!" << endl;
			return -1;
		}
		else
		{
			filetop[min].push_back(tmp);
		}

		if (readover[min] && filetop[min].empty())
		{
			sortover[min] = 1;
			sortovernum++;
			cout << "-------------File " << min << " Sort Over. sortovernum=" << sortovernum << endl;
		}

		DisplayProgressBar("External Sorting", ++time, total);
	}

	ofile.close();
	cout << "\nEnternal Sorting Completed." << endl;
	return 0;
}
