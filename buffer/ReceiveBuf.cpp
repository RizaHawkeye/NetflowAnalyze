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
int ReceiveBuf::moveTo(ReceiveBuf& buf,size_t num)
{
	buf._pkthdrVec = _pkthdrVec;
	int numOfMoved = _pktQue.moveTo(buf._pktQue,num);
	if(numOfMoved == -1)
		return -1;
	else
		return numOfMoved;
}

size_t ReceiveBuf::size()
{
	return _pkthdrQue.size();
}
