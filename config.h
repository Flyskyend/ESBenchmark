#ifndef CONFIG
#define CONFIG

#define CALTIME(time_begin, time_end) \
    time_end.tv_sec - time_begin.tv_sec + 1.0 * (time_end.tv_usec - time_begin.tv_usec) / 1000000

#define toGsize(size) \
	(size * 1024 * 1024 *1024ul)

#define getTracenum(filesize_g) \
	(long)(toGsize(filesize_g) / (sizeof(trace)+(TRACE_HEAD_NUM + TRACE_DATA_NUM)))

#define TRACE_HEAD_NUM 100
#define TRACE_DATA_NUM 5000
//#define TRACE_FILE_SIZE (0.01 * 1024 * 1024 *1024ul)
//#define TRACE_FILE_SIZE_ARRAY_G {1, 2, 5, 10, 20}
#define TRACE_FILE_SIZE_ARRAY_G {0.1, 0.2}
//#define TRACE_FILE_NUM 10
//#define TRACE_FILE_NUM_ARRAY {10000, 5000, 1000, 500, 100, 20, 10, 5, 2}
#define TRACE_FILE_NUM_ARRAY {20, 10, 5, 2}
#define TRACE_SORT_ITEM_INDEX 0
#define CREATE_TRACE_FILE 1
#define FILETOP_BUFFER_SIZE 5
#define MULTI_THREAD_ADDER 1
#define USE_LOSER_TREE 1
#define USE_DEFAULT 1
#define SEPARATOR " "
//#define TRACE_NUM_IN_FILE TRACE_FILE_SIZE / (sizeof(trace) + (TRACE_HEAD_NUM + TRACE_DATA_NUM) / 2)

#endif
