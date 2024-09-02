CFLAG= -I/home/ffengc/Project/Multi-Pattern-Logging-System/base/
LFLAG= -lpthread -lgtest
example.out: example.cc
	g++ -g -std=c++11 $(CFLAG) $^ -o $@  $(LFLAG)
.PHONY:clean
clean:
	rm -rf example.out logfile