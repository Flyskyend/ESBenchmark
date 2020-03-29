#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <sys/time.h>
#include "config.h"
#include "time_recorder.h"

using namespace std;

void TimeRecorder::AddTRBegin(string content)
{
	auto index = map_TRIndex.find(content);
	if (index == map_TRIndex.end())
	{
		timerec tmp;
		tmp.content = content;
		gettimeofday(&tmp.t_begin, NULL);
		TRList.push_back(tmp);
		map_TRIndex.insert(pair<string, int>(content, TRList.size()-1));
	}
	else
	{
		gettimeofday(&TRList[index->second].t_begin, NULL);
	}
}

void TimeRecorder::AddTREnd(string content)
{
	auto index = map_TRIndex.find(content);
	if (index == map_TRIndex.end())
	{
		cout << "Cannot Find Time Recorder Content:" << content << endl;
	}
	else
	{
		int i = index->second;
		gettimeofday(&TRList[i].t_end, NULL);
		TRList[i].t_use = CALTIME(TRList[i].t_begin, TRList[i].t_end);
		printf("%s : %.2fs\n",TRList[i].content.c_str(),TRList[i].t_use);
	}
}

timerec TimeRecorder::GetTR(string content)
{
	auto index = map_TRIndex.find(content);
	if (index == map_TRIndex.end())
	{
		cout << "Cannot Find Time Recorder Content:" << content << endl;
	}
	else
	{
		int i = index->second;
		return TRList[i];
	}
}

double TimeRecorder::GetTRTimeuse(string content)
{
	auto index = map_TRIndex.find(content);
	if (index == map_TRIndex.end())
	{
		cout << "Cannot Find Time Recorder Content:" << content << endl;
	}
	else
	{
		int i = index->second;
		return TRList[i].t_use;
	}
}