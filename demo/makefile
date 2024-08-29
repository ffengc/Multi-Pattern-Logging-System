.PHONY:all
all: args factory
args: args.cc
	g++ -o $@ $^ -std=c++11
factory: factory.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -f args factory