#include "SharedBuf.h"

SharedBuf::SharedBuf():_bufLock(PTHREAD_MUTEX_INITIALIZER)
{ }

SharedBuf::~SharedBuf()
{
	//TODO:what it will do if the lock is destroied
	pthread_rwlock_destroy(&_bufLock);
}

Packet* pop()
{
	pcap_pkthdr* pkthdr = _pkthdrQue[0];
	_pkthdrQue.pop_front();
	
	u_char* pktQue = 0;
	_pktQue.deQueue(pktQue,pkthdr->caplen);
	Packet* pkt = new Packet(pkthdr,pktQue);
}
