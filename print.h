#ifndef PRINT
#define PRINT

#include <iostream>
#include <string>
#include <vector>
#include <stdarg.h>
#include "trace.h"

void PrintTrace(trace *trace, int type);
void PrintTraceList(std::vector<trace> *tracelist);
void DisplayProgressBar(std::string cont, long now, long total);
void PrintandWrite(std::ofstream &resultfile, const char *format, ...);

#endif