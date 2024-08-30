CFLAG= -I./include
LFLAG= -lpthread -lgtest
test: ./src/test.cc
	g++ -g -std=c++11 $(CFLAG) $^ -o ./bin/$@  $(LFLAG)
.PHONY:clean
clean:
	rm -f ./bin/test