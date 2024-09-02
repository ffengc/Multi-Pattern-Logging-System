CFLAG= -I../
LFLAG= -lpthread -lgtest
test: test.cc
	g++ -g -std=c++11 $(CFLAG) $^ -o $@  $(LFLAG)
.PHONY:clean
clean:
	rm -rf test logfile