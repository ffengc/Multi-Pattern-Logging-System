CFLAG= -I../base/
LFLAG= -lpthread
example.out: example.cc
	g++ -g -std=c++11 $(CFLAG) $^ -o $@  $(LFLAG)
.PHONY:clean
clean:
	rm -rf example.out logfile