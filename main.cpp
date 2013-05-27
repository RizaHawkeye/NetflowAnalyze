#include<string>
#include<iostream>
#include"libpcap/LibpcapClass.h"
#include<pthread.h>
void* storeThread(void* args);
int main()
{
	if(LibpcapClass::initDevice() == -1)
	{
		std::cerr<<"Init network interface error"<<std::endl;
	}
	LibpcapClass* libpcapIns = LibpcapClass::instance();

	SharedBuf* sharedBuf = new SharedBuf();
	//TODO:create a new thread to store
	pthread_t tid;
	void* arg = static_cast<void*>(sharedBuf);
	pthread_create(&tid,NULL,storeThread,arg);

	///begin to receive packets
	libpcapIns->beginReceive(sharedBuf);

	if(sharedBuf != 0)
		delete sharedBuf;
	return 0;
}


