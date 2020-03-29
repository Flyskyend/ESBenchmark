#ifndef FILE
#define FILE

#include <iostream>
#include <vector>

int CreateTraceFiles(std::vector<trace> *tracelist, int trace_file_num, double filesize_g);
int ReadOneFromFile(std::ifstream &ifile, trace *trace);
int WriteOneToFile(std::ofstream &ofile, trace *trace);

#endif