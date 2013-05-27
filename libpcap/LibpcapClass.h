#ifndef _QR_LIBPCAPCLASS_H_
#define _QR_LIBPCAPCLASS_H_
#include<netinet/tcp.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include"../buffer/ReceiveBuf.h"
#include"../buffer/SharedBuf.h"
class LibpcapClass
{
public:
	typedef unsigned short u_int16;
	typedef unsigned long u_int32;
	typedef unsigned char u_char;

	static LibpcapClass* instance();
	int beginReceive(SharedBuf*);
	void setFilterString(const string&);

	static int dataLinkOffset(int& protocal,size_t& offset);
	void setAggregateBuf(ReceiveBuf* aggBuf);
	static int initDevice();

private:
	static LibpcapClass* _instance;
	LibpcapClass();

	static pcap_t*  device;
	const static int MAXBYTES2CAPTURE=2048;
	const static size_t RECVBUFTHRESHOLD = 1000;
	string _filterString;
	//CircularQueue<char> _pktQue;
	//CircularQueue<pcap_pkthdr> _pkthdrQue;
	//vector<pcap_pkthdr*> _pkthdrVec;

	/*TODO:_rb应该改成用智能指针的
	 *_rb肯定是要在堆上的，不然在栈上要被销毁的
	 *在堆上何时销毁就成了个问题
	 */
	SharedBuf* _sharedBuf;
	ReceiveBuf* _recvBuf;
	//pcap_loop's callback function

	//friend functions
	/*********************************************
	 *收到的数据包先放入缓存，达到阈值时放入目标队列
	 *如果目标队列满了，就插入一半试试
	 *对目标队列的操作要加锁
	 *???这里push_back pkt 又没为问题？？pkt是在堆上的吗
	 *********************************************/
	friend void _callBackProcess(u_char* userarg,const struct pcap_pkthdr* pkthdr,const u_char* packet)
	{
		LibpcapClass* libpcap = LibpcapClass::instance();

		//malloc heap memory to store data
		pcap_pkthdr* newPkthdr = new pcap_pkthdr();
		memcpy(newPkthdr,pkthdr,sizeof(pcap_pkthdr));
		u_char* newPacket = new u_char[newPkthdr->caplen];
		memcpy(newPacket,packet,sizeof(newPkthdr->caplen));
		///store int buf
		//int numOfEnque = _recvBuf->_pktQue.enQueue(packet,pkthdr->caplen);
		int numOfEnque = libpcap->_recvBuf->addPkt(newPacket,newPkthdr->caplen);
		if(numOfEnque != -1)
		{
			libpcap->_recvBuf->addPkthdr(newPkthdr);
		}
		if(libpcap->_recvBuf->size() >= LibpcapClass::RECVBUFTHRESHOLD)
		{
			///加锁
			libpcap->_sharedBuf->lock();
			//pthread_mutex_lock(_sharedBuf->_bufLock);
			int numOfMoved = libpcap->_recvBuf->moveTo(libpcap->_sharedBuf,LibpcapClass::RECVBUFTHRESHOLD);
			///如果目标队列满了，就插入一半试试
			if(numOfMoved == -1)
			{
				numOfMoved = libpcap->_recvBuf->moveTo(libpcap->_sharedBuf,LibpcapClass::RECVBUFTHRESHOLD/2);
				//TODO:what's will be if the buf is still full
			}
			libpcap->_sharedBuf->unlock();
		}
	}
};

#endif
