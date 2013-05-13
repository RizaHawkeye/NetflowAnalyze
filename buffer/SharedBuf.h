/***************************************************
 *调用lock对ShareBuf加锁，调用unlock对ShareBuf解锁
 *
 **************************************************/
#ifndef _QR_SHAREDBUF_H_
#define _QR_SHAREDBUF_H_
#include "ReceiveBuf.h"
#include<pthread.h>
#include "../packet/Packet.h"

class SharedBuf:public ReceiveBuf
{
public:
	SharedBuf():_bufLock(PTHREAD_MUTEX_INITIALIZER);
	~SharedBuf();
	Packet* pop();
	int lock();
	int unlock();
private:
	pthread_mutex_t _bufLock;
};

#endif
