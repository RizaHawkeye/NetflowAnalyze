#ifndef _QR_RECEIVEBUF_H_
#define _QR_RECEIVEBUF_H_
#include "CircularQueue.h"
#include<deque>
#include<pcap.h>
#include<cstddef.h>
using std::deque;
class ReceiveBuf
{
public:
	typename unsigned char u_char;
	CircularQueue<u_char> _pktQue;
	deque<pcap_pkthdr*> _pkthdrQue
	ReceiveBuf();
	ReceiveBuf(size_t);
	int ReceiveBuf::moveTo(ReceiveBuf& buf,num);
	virtual ~ReceiveBuf();
};

#endif
