#include "Packet.h"
#include<stdexcept>
#include"../libpcap/LibpcapClass.h"
#include<time.h>
#include<sstream>
Packet::Packet(pcap_pkthdr* pkthdr,u_char* packet):_packet(packet),_length(_pkthdrPtr->caplen),
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


/******************************************************
 *free the memory
 ******************************************************/
Packet::~Packet()
{
	if(_packet != 0)
		delete _packet;
	if(_pkthdrPtr != 0)
		delete _pkthdrPtr;
}


/********************************************************
 *
 ********************************************************/
int Packet::_initDatalinkLayer()
{
	if(_setDatalinkLayerProtocalName(_dllprotocal))
		return 0;
	else
		return -1;
}
bool Packet::_setDatalinkLayerProtocalName(int dllprotocal)
{
	switch(dllprotocal)
	{
	case DLT_EN10MB:
		_dllProtocalName = "EN10MB";
		return true;
//	case DLT_IEEE802:
//		//TODO:other prototal
//		break;
	default:
		return false;
	}
}
int Packet::_initNetworkLayer()
{
//	switch(_dllProtocalName)
//	{
//	case "EN10MB":
//		_nlPtr = _dllPtr + 14;
//		_nlprotocal = (ether_head*)_dllPtr->ether_type;
//		if(setNetworkLayerProtocalName(_nlprotocal))
//		{
//			return 0;
//		}
//	default:
//		return -1;
//	}
	if(_dllProtocalName == "EN10MB")
	{
		_nlPtr = _dllPtr + 14;
		_nlprotocal = ((ether_header*)_dllPtr)->ether_type;
		if(_setNetworkLayerProtocalName(_nlprotocal))
		{
			return 0;
		}
		else
			return -1;
	}
	else
		return -1;
}

bool Packet::_setNetworkLayerProtocalName(u_int16 nlprotocal)
{
	switch(nlprotocal)
	{
	case 0x0800:	 //ip
		_nlProtocalName = "IPv4";
		return true;
	case 0x86DD:    //ipv6
		_nlProtocalName = "IPv6";
		return true;
	default:
		return false;
	}
}

int Packet::_initTransportLayer()
{
//	switch(_nlProtocal)
//	{
//	case "IPv4":
//		_tlPtr = _nlPtr + (iphdr*)_nlPtr->ihl;
//		_tlprotocal = (iphdr*)_nlPtr->protocol;
//		//_tlprotocal = _iphdrPtr->protocol;
//		_setTransportLayerProtocalName(_tlprotocal);
//		return 0;
//	case "IPv6":
//		//TODO:
//		_setTransportLayerProtocalName(_tlprotocal);
//		return 0;
//	default:
//		return -1;
//	}
	if(_nlProtocalName == "IPv4")
	{
		_tlPtr = _nlPtr + ((iphdr*)_nlPtr)->ihl;
		_tlprotocal = ((iphdr*)_nlPtr)->protocol;
		//_tlprotocal = _iphdrPtr->protocol;
		_setTransportLayerProtocalName(_tlprotocal);
		return 0;
	}
	else if(_nlProtocalName == "IPv6")
	{
		//TODO:
		_setTransportLayerProtocalName(_tlprotocal);
		return 0;
	}
	else
		return -1;
}
bool Packet::_setTransportLayerProtocalName(u_char tlprotocal)
{
	switch(tlprotocal)
	{
	case 0x06:   //tcp
		_tlProtocalName = "TCP";
		return 0;
	case 0x11:   //udp
		_tlProtocalName = "UDP";
		return 0;
	default:
		return -1;
	}
}
Packet::u_int32 Packet::getSrcIpv4() const
{
	if(_nlProtocalName == "IPv4")
		return ((iphdr*)_nlPtr)->saddr;
	else
		//TODO:throw an exception;
		throw std::runtime_error("不是IP4协议");
}

Packet::u_int32 Packet::getDstIpv4() const
{
	if(_nlProtocalName == "IPv4")
		return ((iphdr*)_nlPtr)->daddr;
	else
		//TODO:throw an exception;
		throw std::runtime_error("不是IP4协议");
}
in6_addr Packet::getSrcIpv6() const
{
	if(_nlProtocalName == "IPv6")
		return ((ip6_hdr*)_nlPtr)->ip6_src;
	else
		//TODO:throw an exception;
		throw std::runtime_error("不是IP6协议");
}
in6_addr Packet::getDstIpv6() const
{
	if(_nlProtocalName == "IPv6")
		return ((ip6_hdr*)_nlPtr)->ip6_dst;
	else
		//TODO:throw an exception;
		throw std::runtime_error("不是IP6协议");
}

/*************************************
 *if error return NULL string
 *inet_ntoa and inet_ntop can change the network order to host order
 ************************************/
string Packet::getSource() const
{
	if(_nlProtocalName == "IPv4")
	{
		try
		{
			in_addr ipn;
			ipn.s_addr = getSrcIpv4();
			char* ipa = inet_ntoa(ipn);
			return string(ipa);
		}
		catch(std::exception& ex)
		{
			return "";	
		}
	}
	else if(_nlProtocalName == "IPv6")
	{
		try{
			char ipv6[INET6_ADDRSTRLEN];
			in6_addr ipv6n = getSrcIpv6();
			const char* ret = inet_ntop(AF_INET6,static_cast<void*>(&ipv6n),ipv6,INET6_ADDRSTRLEN);
			if(ret != NULL)
			{
				return string(ipv6);
			}
			else
			{
				return "";
			}
		}
		catch(std::exception& ex)
		{
			return "";
		}
	}
	else
		return "";
}

string Packet::getDestination() const
{
	if(_nlProtocalName == "IPv4")
	{
		try
		{
			in_addr ipn;
			ipn.s_addr = getDstIpv4();
			char* ipa = inet_ntoa(ipn);
			return string(ipa);
		}
		catch(std::exception& ex)
		{
			return "";
		}
	}
	else if(_nlProtocalName == "IPv6")
	{
		try{
			char ipv6[INET6_ADDRSTRLEN];
			in6_addr ipv6n = getDstIpv6();
			const char* ret = inet_ntop(AF_INET6,static_cast<void*>(&ipv6n),ipv6,INET6_ADDRSTRLEN);
			if(ret != NULL)
			{
				return string(ipv6);
			}
			else
			{
				return "";
			}
		}
		catch(std::exception& ex)
		{
			return "";
		}
	}
	else
	{
		return "";
	}
}

string Packet::getTime() const
{
	std::ostringstream sout;
	sout<<_pkthdrPtr->ts.tv_sec<<"."<<_pkthdrPtr->ts.tv_usec;
	return sout.str();
}
/****************************************
 *if this packet has transfer layer protocal return it
 *if it doesn't have transfer layer protocal and it has network layer return it
 ****************************************/
string Packet::getProtocal() const
{
	if(_tlProtocalName != "")
		return _tlProtocalName;
	else if(_nlProtocalName != "")
		return _nlProtocalName;
	else
		return _dllProtocalName;
}
size_t Packet::getLength() const
{
	return _length;
}
