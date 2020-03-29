#ifndef TIME_RECORDER_
#define TIME_RECORDER_

#include <map>
#include <vector>
#include <sys/time.h>

using namespace std;

struct timerec
{
	string content;
	timeval t_begin;
	timeval t_end;
	double t_use;
};

class TimeRecorder
{
	public:
		map<string,int> map_TRIndex;
		vector<timerec> TRList;

		//TimeRecorder();
		void AddTRBegin(string content);
		void AddTREnd(string content);
		timerec GetTR(string content);
		double GetTRTimeuse(string content);


};



#endif 

