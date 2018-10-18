all: main.cpp model.cpp valueIterator.cpp
	g++ -std=c++11 -o main main.cpp model.cpp valueIterator.cpp