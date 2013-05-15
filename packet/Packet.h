/**************************************************
 *_packet point to the packet
 **************************************************/
#ifndef _QR_PACKET_
#define _QR_PACKET_
#include<string>
#include<pcap.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<net/ethernet.h>
using std::string;
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
	string _dllProtocalName;

	///network layer
	u_char* _nlPtr;
	u_int16 _nlprotocal;
	string _nlProtocalName;

	//iphdr* _iphdrPtr;
	//ip6_hdr* _ip6hdrPtr;

	///transport layer
	u_char* _tlPtr;
	u_char _tlprotocal;
	string _tlProtocalName;
	//tcphdr* _tcphdrPtr;
	//udphdr* _udphdrPtr;
	
	///if return -1 means unrecognise the datalink layer protocal
	int _initDatalinkLayer();

	///if return -1 means unrecognise the network layer protocal
	int _initNetworkLayer();

	///if return -1 means unrecognise the transport layer protocal
	int _initTransportLayer();

	///this set of functions return false if protocal is unrecognised
	bool _setDatalinkLayerProtocalName(int dllprotocal)
	bool _setNetworkLayerProtocalName(u_int16 nlprotocal)
	bool _setTransportLayerProtocalName(u_char tlprotocal)


public:
	Packet();
	//TODO:I perfer to return string or add another four function to return string
	u_int32 getSrcIpv4();
	u_int32 getDstIpv4();
	in6_add getSrcIpv6();
	in6_add getDstIpv6();

	//TODO: how to change to string
	///this set of functions used to store
	string getSource();
	string getDestination();
	string getTime();
	string getProtocal();
	size_t getLength();
};
#endif
