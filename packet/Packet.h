#ifndef _QR_PACKET_
#define _QR_PACKET_
#include<pcap.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<net/ethernet.h>
class Packet
{
public:
	typename unsigned char u_char;
	typename unsigned short int u_int16;
	typename unsigned long u_int32;
private:
	u_char* _packet;
	size_t _length;
	pcap_pkthdr* _pkthdrPtr;
	///datalink layer
	u_char* _dllPtr;
	int _dllprotocal;
	size_t _dlloffset;
	///network layer
	u_char* _nlPtr;
	u_int16 _nlprotocal;
	//iphdr* _iphdrPtr;
	//ip6_hdr* _ip6hdrPtr;
	///transport layer
	u_char* _tlPtr;
	u_char _tlprotocal;
	//tcphdr* _tcphdrPtr;
	//udphdr* _udphdrPtr;
	
	///if return -1 means unrecognise the dll protocal
	int _initNetworkLayer();

	///if return -1 means unrecognise the dll protocal
	int _initTransportLayer();
public:
	Packet();
	int srcIpv4(u_int32&);
	int dstIpv4(u_int32&);
	int srcPort(u_int16&);
	int dstPort(u_int16&);
};
#endif
