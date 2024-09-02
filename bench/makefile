CFLAG= -I/home/ffengc/Project/Multi-Pattern-Logging-System/base/
LFLAG= -lpthread -lgtest
bench.out: bench.cc
	g++ -g -std=c++11 $(CFLAG) $^ -o $@  $(LFLAG)
.PHONY:clean
clean:
	rm -rf bench.out logfile