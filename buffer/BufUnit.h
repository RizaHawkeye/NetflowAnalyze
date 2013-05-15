/************************************************
 *
 *
 ***********************************************/
#ifdef _QR_BUFUNIT_H_
#define _QR_BUFUNIT_H_
#include<vector>
class BufUnit
{
public:
	typename unsigned char u_char;
private:
	u_char* bufPos;
	pcap_pkthdr* bufHead;
};


#endif
