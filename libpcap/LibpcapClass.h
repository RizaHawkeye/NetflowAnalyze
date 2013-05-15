#ifndef _QR_LIBPCAPCLASS_
#define _QR_LIBPCAPCLASS_
#include<netinet/tcp.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include"buffer/ReceiveBuf.h"
#include"buffer/SharedBuf.h"
class LibpcapClass
{
public:
	typedef unsigned short u_int16;
	typedef unsigned long u_int32;
	typedef unsigned char u_char;
	LibpcapClass();
	int beginReceive();
	void setFilterString(const string&);

	static int dataLinkOffset(int& protocal,size_t& offset);
	void setAggregateBuf(ReceiveBuf* aggBuf);
private:
	static pcap_t*  device;
	static int initDevice();
	size_t RECVBUFTHRESHOLD = 1000;
	string _filterString;
	//CircularQueue<char> _pktQue;
	//CircularQueue<pcap_pkthdr> _pkthdrQue;
	//vector<pcap_pkthdr*> _pkthdrVec;
	
	/*TODO:_rb应该改成用智能指针的
	 *_rb肯定是要在堆上的，不然在栈上要被销毁的
	 *在堆上何时销毁就成了个问题
	 */
	SharedBuf* _aggBuf;
	ReceiveBuf* _recvBuf;
	//pcap_loop's callback function
	void _callBackProcess(u_char*,const struct pcap_ptkhdr,const u_char*);
};
#endif
