

#ifndef UDPMEDIATOR_H
#define UDPMEDIATOR_H

#include "IMediator.h"
#include "UDPNet.h"

class UDPMediator;
typedef void (UDPMediator:: *PFUN)(char*,long );
struct ProtocolMap
{
	int m_nProtocol;
	PFUN m_pProtocolMap;
};

class UDPMediator :public IMediator
{
public:
	UDPMediator(CWnd *pWnd);
	virtual~UDPMediator();
public:
	 bool open() ;
	 void close();
	 bool SendData(long lSendIp,char *szbuf,int nlen);
	 void DealData(char* szbuf,long lRecvIp);
public:
	 void DealOnLineRq(char* szbuf,long lRecvIp);
	 void DealOnLineRs(char* szbuf,long lRecvIp);
	 void DealOffLineNTF(char* szbuf,long lRecvIp);
	  void DealDataInfo(char* szbuf,long lRecvIp);
private:
	INet *m_pudpNet;
	CWnd *m_pWnd;
	PFUN  m_protocolMap[_MAX_PROTOCOL_LEN];

};








#endif
