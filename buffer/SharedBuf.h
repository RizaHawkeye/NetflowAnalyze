#ifndef _QR_SHAREDBUF_H_
#define _QR_SHAREDBUF_H_
#include "ReceiveBuf.h"
#include<pthread.h>
#include "../packet/Packet.h"

class SharedBuf:public ReceiveBuf
{
public:
	pthread_mutex_t _bufLock;
	SharedBuf():_bufLock(PTHREAD_MUTEX_INITIALIZER);
	~SharedBuf();
	Packet* pop();
};

#endif
