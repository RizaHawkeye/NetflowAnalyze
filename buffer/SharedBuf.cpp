#include "SharedBuf.h"

SharedBuf::SharedBuf():_bufLock(PTHREAD_MUTEX_INITIALIZER)
{ }

SharedBuf::~SharedBuf()
{
	//TODO:what it will do if the lock is destroied? How to judge if lock had destroy
	pthread_mutex_destroy(&_bufLock);
}

/**************************
 *delete from ShareBuf and construct a Packet object
 *be attention to delete Packet object
 *************************/
Packet* SharedBuf::pop()
{
	pcap_pkthdr* pkthdr = new pcap_pkthdr();
	*pkthdr = _pkthdrQue[0];
	_pkthdrQue.pop_front();
	
	u_char* pkt = new u_char[pkthdr->caplen];
	_pktQue.deQueue(pkt,pkthdr->caplen);
	Packet* pkt = new Packet(pkthdr,pktQue);
}

int SharedBuf::lock()
{
	int ret = pthread_mutex_lock(_bufLock)
	return ret;
}
int SharedBuf::unlock()
{
	int ret = pthread_mytex_unlock(_bufLock)
	return ret;
}
