/**************************************************
 *_packet point to the packet
 *system head files in /usr/include/net/ethernet.h
 **************************************************/
#ifndef _QR_PACKET_
#define _QR_PACKET_
#include<string>
#include<pcap.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<net/ethernet.h>
#include<netinet/ip6.h>
#include<arpa/inet.h>   //inet_aton  inet_ntop
#include<netinet/in.h>  //htonl
using std::string;
class Packet
{
public:
	typedef unsigned char u_char;
	typedef unsigned short int u_int16;
	typedef unsigned long u_int32;
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
	bool _setDatalinkLayerProtocalName(int dllprotocal);
	bool _setNetworkLayerProtocalName(u_int16 nlprotocal);
	bool _setTransportLayerProtocalName(u_char tlprotocal);


public:
	Packet(pcap_pkthdr*,u_char*);
	~Packet();
	//TODO:I perfer to return string or add another four function to return string
	u_int32 getSrcIpv4() const;
	u_int32 getDstIpv4() const;
	in6_addr getSrcIpv6() const;
	in6_addr getDstIpv6() const;

	//TODO: how to change to string
	///this set of functions used to store
	string getSource() const;
	string getDestination() const;
	string getTime() const;
	string getProtocal() const;
	size_t getLength() const;
};
#endif
