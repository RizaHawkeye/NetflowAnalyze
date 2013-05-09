#include "Packet.h"
#include<stdexcept>

Packet::Packet(pcap_pkthdr* pkthdr,u_char* packet):_packet(packet),_length(_pkthdePtr->caplen),
											_pkthdrPtr(pkthdr), _dllPtr(0),_dllprotocal(0),
											_dlloffset(0), _nlPtr(0),_nlprotocal(0),
											_tlPtr(0), _tlprotocal(0)
{
	memcpy((void*)_packet,(void*)packet,pkthdr->caplen);
	///TODO:获得链路层协议的协议头
	if(LibpcapClass::dataLinkOffset(_dllprotocal,_dlloffset) != 0)
	{
		throw std::runtime_error("未识别的链路层协议");
	}

	if(_initNetworkLayer() == -1)
	{
		throw std::runtime_error("未识别的链路层协议");
	}
	if(_initTransportLayer() == -1)
	{
		throw std::runtime_error("未识别的网络层协议");
	}
}

int Packet::_initNetworkLayer()
{
	switch(_dllprotocal)
	{
	case DLT_EN10MB:
		_nlPtr = _dllPtr + 14;
		_nlprotocal = (ether_head*)_dllPtr->ether_type;
		return 0;
//	case DLT_IEEE802:
//		//TODO:other prototal
//		break;
	default:
		return -1;
	}

}

int Packet::_initTransportLayer()
{
	switch(_nlprotocal)
	{
	////ip
	case 0x0800:
		_tlPtr = _nlPtr + (iphdr*)_nlPtr->ihl;
		_tlprotocal = (iphdr*)_nlPtr->protocol;
		//_tlprotocal = _iphdrPtr->protocol;
		return 0;
		break;
//	//ipv6
//	case 0x86DD:
//		//TODO:
//		break;
	default:
		return -1;
	}
}

int Packet::srcIpv4(u_int32& ip)
{
	///ip
	if(_nlprotocal == 0x0800)
	{
		ip = _nlPtr->saddr;
		return 0;
	}
	else
		return -1;
}

int Packet::dstIpv4(u_int32& ip)
{
	if(_nlprotocal == 0x0800)
	{
		ip = _nlPtr->daddr;
		return 0;
	}
	else
		return -1;
}

int Packet::srcPort(u_int16& port)
{
	if(_tlprotocal == 0x06)   //tcp
	{
		port = _tlPtr->source;
		return 0;
	}
	else if(0x11) //udp
	{
		port = _tlPtr->source;
		return 0;
	}
	else
		return -1;
}

int Packet::dstPort(u_int16& port)
{
	if(_tlprotocal == 0x06)   //tcp
	{
		port = _tlPtr->dest;
		return 0;
	}
	else if(0x11) //udp
	{
		port = _tlPtr->dest;
		return 0;
	}
	else
		return -1;
}







