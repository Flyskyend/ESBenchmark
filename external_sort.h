#ifndef EXTERNAL_SORT
#define EXTERNAL_SORT

//class LoserTree;

#include <mutex>

using namespace std;

//mutex mtx[TRACE_FILE_NUM];

int CheckFiles(vector<ifstream> &ifiles, int trace_file_num);
int FindMin(vector<vector<trace>> filetop, vector<int> &sortover, int trace_file_num);
int LoadBuffer(int trace_file_num, int filetop_buffer_size, vector<int> &readover, int &readovernum, vector<ifstream> &ifiles, vector<vector<trace>> &filetop);
int SortFiles_LoserTree_MT(int trace_file_num, double trace_file_size_g);
int SortFiles_Default_MT(int trace_file_num, double trace_file_size_g);
int SortFiles_LoserTree_ST(int trace_file_num, double trace_file_size_g);
int SortFiles_Default_ST(int trace_file_num, double trace_file_size_g);

#endif

