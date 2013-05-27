#include"ReceiveBuf.h"

ReceiveBuf::ReceiveBuf(size_t size):_pktQue(size)
{ }

ReceiveBuf::ReceiveBuf()
{}

ReceiveBuf::~ReceiveBuf()
{

}
/***************************************************
 *将当前ReceiveBuf中的所有内容移动到参数buf中
 *if buf is full ,then return -1;
 ***************************************************/
int ReceiveBuf::moveTo(ReceiveBuf* buf,size_t num)
{
	buf->_pkthdrQue = _pkthdrQue;
	int numOfMoved = _pktQue.moveTo(buf->_pktQue,num);
	if(numOfMoved == -1)
		return -1;
	else
		return numOfMoved;
}

size_t ReceiveBuf::size()
{
	return _pkthdrQue.size();
}
int ReceiveBuf::addPkt( u_char* pkt,size_t num)
{
	return _pktQue.enQueue(pkt,num);
}

int ReceiveBuf::addPkthdr( pcap_pkthdr* pkthdr)
{
	_pkthdrQue.push_back(pkthdr);
	return 0;
}
