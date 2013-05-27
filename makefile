outfile = debug/

Main = $(outfile)main.o
LibpcapClass = $(outfile)LibpcapClass.o
Log = $(outfile)Log.o
Packet = $(outfile)Packet.o
Store = $(outfile)Store.o
ReceiveBuf = $(outfile)ReceiveBuf.o
SharedBuf = $(outfile)SharedBuf.o

objects = $(Main) $(LibpcapClass) $(Log) $(Packet) $(Store) $(ReceiveBuf) $(SharedBuf)

#objects = main.o LibpcapClass.o Log.o Packet.o Store.o ReceiveBuf.o SharedBuf.o

a.out:  $(objects)
	g++ -o debug/a.out -Wall $(objects) -lpcap -lpthread -g
	
$(Main): main.cpp libpcap/LibpcapClass.h
	g++ -c -o $(Main) -Wall main.cpp

$(LibpcapClass): libpcap/LibpcapClass.h libpcap/LibpcapClass.cpp buffer/ReceiveBuf.h buffer/SharedBuf.h
	g++ -c -o $(LibpcapClass) -Wall libpcap/LibpcapClass.cpp

$(Log): log/Log.h log/Log.cpp
	g++ -c -o $(Log) -Wall log/Log.cpp

$(Packet): packet/Packet.h packet/Packet.cpp libpcap/LibpcapClass.h
	g++ -c -o $(Packet) -Wall packet/Packet.cpp

$(Store): Store.cpp buffer/SharedBuf.h packet/Packet.h
	g++ -c -o $(Store) -Wall Store.cpp

$(ReceiveBuf): buffer/ReceiveBuf.h buffer/ReceiveBuf.cpp buffer/CircularQueue.h
	g++ -c -o $(ReceiveBuf) -Wall buffer/ReceiveBuf.cpp

$(SharedBuf): buffer/ReceiveBuf.h packet/Packet.h buffer/SharedBuf.h
	g++ -c -o $(SharedBuf) -Wall buffer/SharedBuf.cpp
	
clean: 
	rm a.out* $(objects)
