#include <fstream>
#include "adder_thread.h"
#include "trace.h"
#include "file.h"

using namespace std;

void FiletopAdderThread_AddNeed(struct thread_data *arg)
{
	//sleep(10);
	trace tmp;

	cout << "Filetop Adder Thread Created." << endl;
	//cout<<arg->filetop<<endl;

	while (*(arg->readovernum) < arg->trace_file_num)
	{
		if (*(arg->needread))
		{
			int min = *(arg->readindex);
			if (arg->filetop->at(min).size() < FILETOP_BUFFER_SIZE && !arg->readover->at(min))
			{
				//cout<<"file"<<i<<":"<<arg->filetop->at(i).size();
				int ret = ReadOneFromFile(arg->ifiles->at(min), &tmp);
				if (ret == 1)
				{
					arg->readover->at(min) = 1;
					*(arg->readovernum) += 1;
					arg->ifiles->at(min).close();
					cout << " ------------------------File " << min << " Read Over. readovernum=" << *(arg->readovernum) << endl;
				}
				else if (ret == -1)
				{
					cout << "Read One From File: " << min << " Error !!" << endl;
				}
				else
				{
					//PrintTrace(&toplist[min],0);
					cout << "#";
					arg->filetop->at(min).push_back(tmp);
					*(arg->needread) = 0;
					//cout<<" -> file"<<i<<":"<<arg->filetop->at(i).size()<<endl;
					//cout<<"File "<<min<<": "<<filetop[min].size()<<endl;
				}
			}
		}
	}

	cout << "Filetop Adder Thread Finished." << endl;
	return;
}

void FiletopAdderThread_TraversalCheck(struct thread_data *arg)
{
	//sleep(10);
	int i = 0;
	trace tmp;

	cout << "Thread Creating Completed." << endl;
	//cout<<arg->filetop<<endl;

	while (*(arg->readovernum) < arg->trace_file_num)
	{
		//mtx[i].lock();
		if (arg->filetop->at(i).size() < FILETOP_BUFFER_SIZE && !arg->readover->at(i))
		{
			//cout<<"file"<<i<<":"<<arg->filetop->at(i).size();
			//mtx[i].unlock();
			int ret = ReadOneFromFile(arg->ifiles->at(i), &tmp);
			if (ret == 1)
			{
				arg->readover->at(i) = 1;
				*(arg->readovernum) += 1;
				arg->ifiles->at(i).close();
				cout << " ------------------------File " << i << " Read Over. readovernum=" << *(arg->readovernum) << endl;
			}
			else if (ret == -1)
			{
				cout << "Read One From File: " << i << " Error !!" << endl;
			}
			else
			{
				//PrintTrace(&toplist[min],0);
				//mtx[i].lock();
				arg->filetop->at(i).push_back(tmp);
				//mtx[i].unlock();
				//cout<<" -> file"<<i<<":"<<arg->filetop->at(i).size()<<endl;
				//cout<<"File "<<min<<": "<<filetop[min].size()<<endl;
			}
		}
		else
		{
			//mtx[i].unlock();
			if (++i == arg->trace_file_num)
				i = 0;
		}
		
	}

	cout << "Filetop Adder Thread Finished." << endl;
	return;
}
