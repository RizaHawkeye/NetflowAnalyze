#include<string>
#include<iostream>
#include"../LibpcapClass.h"

int main()
{
	if(LibpcapClass::initDevice() == -1)
	{
		cerr<<"Init network interface error"<<endl;
	}
	LibpcapClass libpcapObj;

	//TODO:create a new thread to store
	pthread_t tid;
	pthread_create(tid,NULL,storeThread,

	///begin to receive packets
	libpcapObj.beginReceive();

	return 0;
}


