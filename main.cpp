#include<string>
#include<iostream>
#include"LibpcapClass.h"

int main()
{
	if(LibpcapClass::initDevice() == -1)
	{
		cerr<<"Init network interface error"<<endl;
	}
	//TODO:create a new thread to store
	LibpcapClass libpcapObj;
	libpcapObj.beginReceive();

	
	return 0;
}
