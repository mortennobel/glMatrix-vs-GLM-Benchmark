CXX = g++
OPT = -O3
CXXFLAGS = -O3
WARN = -Wall

CFLAGS = $(OPT) $(WARN) # the C compiler flags

run: project
	./glmBenchmarkSingle 2000000 10
	./glmBenchmarkDouble 2000000 10


project: $(OBJECTS)
	$(CXX) $(CFLAGS) -o glmBenchmarkSingle Timer.cpp main.cpp
	$(CXX) $(CFLAGS) -D GLM_PRECISION_HIGHP_FLOAT -o glmBenchmarkDouble Timer.cpp main.cpp	

clean:
	@rm -f *.o


