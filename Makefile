all:
	g++ wflood.cpp httpcpp.cpp -o wflood -std=c++11 -Wwrite-strings 

clean:
	rm -f wflood
