#include "LibpcapClass.h"
#include "../log/Log.h"
#include <pthread.h>

pcap_t* LibpcapClass::device = 0;
LibpcapClass* LibpcapClass::_instance = 0;
LibpcapClass::LibpcapClass():_recvBuf(new ReceiveBuf),_sharedBuf(0)
{ }

LibpcapClass* LibpcapClass::instance()
{
	if(_instance == 0)
	{
		_instance = new LibpcapClass();
	}
	return _instance;
}
//void LibpcapClass::setAggregateBuf(SharedBuf* aggBuf)
//{
//	_sharedBuf = aggBuf;
//}

/********************************************
 *begin to receive until something is wrong
 *TODO:I prefer to add a parameter to decide receiving until something or other behaves
 *receive data and store in _recvBuf
 *when the count of data have come to a threshold, move them to _sharedBuf
 *parameter sharedBuf is shared with store thread
 ********************************************/
int LibpcapClass::beginReceive(SharedBuf* sharedBuf)
{
	_sharedBuf = sharedBuf;
	Log log;
	int count = 0;
	char* devStr = 0;

	bpf_u_int32 netaddr = 0,mask = 0;
//	pcap_t* device = NULL;
	struct bpf_program filter;
	struct ip* iphdr = NULL;
	//tcp header
	struct tcphdr* tcphdr = NULL;
	struct pcap_pkthdr pkthdr;
	const unsigned char* packet;

	char errBuf[PCAP_ERRBUF_SIZE];

	if(initDevice() == -1)
		return -1;
	//devStr = pcap_lookupdev(errBuf);
	//if(!devStr)
	//{
	//	string msg = "";
	//	msg += "error(pcap_lookupdev()): ";
	//	msg += errBuf;
	//	log.writeLog(msg);
	//	//cerr<<"error(pcap_lookupdev()): "<<errBuf<<endl;;
	//	exit(1);
	//}

	/////device是静态的
	//if(device == 0)
	//	device = pcap_open_live(devStr,MAXBYTES2CAPTURE,1,512,errBuf);
	//if(!device)
	//{
	//	string msg = "error(pcap_open_live()): ";
	//	msg += errBuf;
	//	log.writeLog(msg);
	//	//cerr<<"error(pcap_open_live()): "<<errBuf<<endl;
	//	exit(1);
	//}

	if(_filterString.size() != 0)
	{
		pcap_lookupnet(devStr,&netaddr,&mask,errBuf);

		//if(-1 == pcap_compile(device,&filter,"(tcp[13] == 0x10) or (tcp[13] == 0x18)",1,mask))
		if(-1 == pcap_compile(device,&filter,_filterString.c_str(),1,mask))
		{
			//cerr<<"error(pcap_compile()) "<<endl;
			string msg = "error(pcap_compile()) ";
			log.writeLog(msg);
		}

		if(-1 == pcap_setfilter(device,&filter))
		{
			//cerr<<"error(pcap_setfilter())"<<endl;
			string msg = "error(pcap_setfilter())";
			log.writeLog(msg);
		}
	}
	//出错时才返回
	pcap_loop(device,-1,_callBackProcess,(u_char*)&count);

	pcap_close(device);
	return 0;
}

///*********************************************
// *收到的数据包先放入缓存，达到阈值时放入目标队列
// *如果目标队列满了，就插入一半试试
// *对目标队列的操作要加锁
// *???这里push_back pkt 又没为问题？？pkt是在堆上的吗
// *********************************************/
//friend void _callBackProcess(u_char* userarg,const struct pcap_pkthdr* pkthdr,const u_char* packet)
//{
//	LibpcapClass* libpcap = LibpcapClass::instance();
//
//	//malloc heap memory to store data
//	pcap_pkthdr* newPkthdr = new pcap_pkthdr();
//	memcpy(newPkthdr,pkthdr,sizeof(pcap_pkthdr));
//	u_char* newPacket = new u_char[newPkthdr->caplen];
//	memcpy(newPacket,packet,sizeof(newPkthdr->caplen));
//	///store int buf
//	//int numOfEnque = _recvBuf->_pktQue.enQueue(packet,pkthdr->caplen);
//	int numOfEnque = libpcap->_recvBuf->addPkt(newPacket,newPkthdr->caplen);
//	if(numOfEnque != -1)
//	{
//		libpcap->_recvBuf->addPkthdr(newPkthdr);
//	}
//	if(libpcap->_recvBuf->size() >= LibpcapClass::RECVBUFTHRESHOLD)
//	{
//		///加锁
//		libpcap->_sharedBuf->lock();
//		//pthread_mutex_lock(_sharedBuf->_bufLock);
//		int numOfMoved = libpcap->_recvBuf->moveTo(libpcap->_sharedBuf,LibpcapClass::RECVBUFTHRESHOLD);
//		///如果目标队列满了，就插入一半试试
//		if(numOfMoved == -1)
//		{
//			numOfMoved = libpcap->_recvBuf->moveTo(libpcap->_sharedBuf,LibpcapClass::RECVBUFTHRESHOLD/2);
//			//TODO:what's will be if the buf is still full
//		}
//		libpcap->_sharedBuf->unlock();
//	}
//}
/***************************************
 *add a filter string for libpcap
 *default value is null string
 ***************************************/
void LibpcapClass::setFilterString(const string& str)
{
	_filterString = str;
}

/********************************************
 *get datalink layer's protocal and offset
 *如果没有该协议，protocal设置为-1，offset设置为0
 ********************************************/
int LibpcapClass::dataLinkOffset(int& protocal,size_t& offset)
{

	if(initDevice() == -1)
	{
		protocal = -1;
		offset = 0;
		return -1;
	}
	//返回由函数pcap_open_live()打开的网络设备的链路层类型
	switch(pcap_datalink(device))
	{
		case DLT_EN10MB:
			protocal = DLT_EN10MB;
			offset = 14;
			break;
		case DLT_IEEE802:
			protocal = DLT_IEEE802;
			offset = 22;
			break;
		case DLT_FDDI:
			protocal = DLT_FDDI;
			offset = 21;
	#ifdef DLT_LOOP
		case DLT_LOOP:
			protocal = DLT_LOOP;
			offset = 4;
			break;
	#endif
		case DLT_NULL:
			protocal = DLT_NULL;
			offset = 4;
			break;
		default:
			protocal = -1;
			offset = 0;
			return -2;
	}
	return 0;
}

/********************************************
 *设置device的值,device是静态的，初始化一次就行了
 *if device have initialized, return
 ********************************************/
int LibpcapClass::initDevice()
{
	Log log;
	///device是静态的，如果device不为0，就不用在调函数了
	if(device != 0)
	{
		return 0;
	}
	char* devStr = 0;
	char errBuf[PCAP_ERRBUF_SIZE];

	memset(errBuf,0,PCAP_ERRBUF_SIZE);

	devStr = pcap_lookupdev(errBuf);
	if(!devStr)
	{
		string msg = "";
		msg += "error(pcap_lookupdev()): ";
		msg += errBuf;
		log.writeLog(msg);
		//cerr<<"error(pcap_lookupdev()): "<<errBuf<<endl;;
		return -1;
	}

	device = pcap_open_live(devStr,MAXBYTES2CAPTURE,1,512,errBuf);
	if(!device)
	{
		string msg = "error(pcap_open_live()): ";
		msg += errBuf;
		log.writeLog(msg);
		//cerr<<"error(pcap_open_live()): "<<errBuf<<endl;
		return -1;
	}
	return 0;
}
