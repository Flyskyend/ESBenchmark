GXX	= g++
LD	= $(GXX)

LIBS	+= -lpthread
INCLUDE += -I./include
CFLAGS = -Wall -O2

SRCS	= $(wildcard *.cpp)
OBJS	= $(patsubst %cpp, %o, $(SRCS))
TARGET	= ESBenchmark

MKDIR	= mkdir -p
CHK_DIR_EXISTS	= test -d
TRACEFILESDIR	= ./TraceFilesDir
RESULTFILE	= ESTraceResult
BENCHMARK_RESULT = ESBenchmarkResult

.PHONY:all clean remkdir cleanall

#.SUFFIXES: .o .cpp
#.o.cpp:
#	$(GXX) -c $(INCLUDE) -o $@ $<

all: clean $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CHK_DIR_EXISTS) $(TRACEFILESDIR) | $(MKDIR) $(TRACEFILESDIR)
	$(LD) $(OBJS) $(LIBS) -o $(TARGET)

%.o:%.cpp
	$(GXX) -c $^ 

remkdir:
	rm -rf $(TRACEFILESDIR)
	mkdir $(TRACEFILESDIR)
	
clean:
	rm -f $(TARGET) $(OBJS)
	rm -f $(RESULTFILE)

cleanall: clean
	rm -rf $(TRACEFILESDIR)
	rm -rf $(BENCHMARK_RESULT)
