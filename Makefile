all:
	g++ wflood.cpp httpcpp.cpp -o wflood -std=c++11

clean:
	rm -f wflood