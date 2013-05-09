all:
	g++ -g -Wall -o tcp tcp.cpp -lpcap
clean:
	rm -rf *.o tcp
