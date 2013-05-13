#include "packet/Packet.h"
#include "buffer/SharedBuf.h"
#include <pthread.h>
#include <fstream>
#include <string>

/******************************************************
 *从缓存中读取数据包，然后存入文本（数据库）中
 *sbuf指向共享缓存
 *****************************************************/
int store(SharedBuf* sbuf)
{
	string outputFile = "~/code/codeblocksProj/NetflowAnalyze/NetflowAnalyze/output.txt";
	ofstream fout(outputFile.c_str());


	//pthread_mutex_lock(sbuf->_bufLock);
	//TODO:when calling the "unlock"
	sbuf->lock();
	
	size_t pktCount = sbuf->_pkthdrVec.size();
	for(size_t i = 0;i<pktCount;++i)
	{
		//TODO:CircularQueue need a function: getPacket(T* start,size_t count,T* target);
		Packet pkt = sbuf->pop();
	
	}

}
