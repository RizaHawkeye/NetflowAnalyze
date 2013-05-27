/***************************************
 *ReceiveBuf used to store all origin packets and the their pcap_pkthdr struct
 *
 ***************************************/
#ifndef _QR_RECEIVEBUF_H_
#define _QR_RECEIVEBUF_H_
#include "CircularQueue.h"
#include<deque>
#include<pcap.h>
#include<cstddef>
using std::deque;
class ReceiveBuf
{
public:
	typedef unsigned char u_char;
	ReceiveBuf();
	ReceiveBuf(size_t);
	int moveTo(ReceiveBuf* buf,size_t num);
	virtual ~ReceiveBuf();
	size_t size();
	int addPkt( u_char*,size_t );
	int addPkthdr( pcap_pkthdr*);
protected:
	CircularQueue<u_char> _pktQue;
	deque<pcap_pkthdr*> _pkthdrQue;
};

#endif
