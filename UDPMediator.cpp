#include "stdafx.h"
#include "UDPMediator.h"

UDPMediator::UDPMediator(CWnd *pWnd)
{
	m_pudpNet = new UDPNet(this);
	m_pWnd = pWnd;


	//m_protocolMap[_DEF_PROTOCOL_ONLINE_RQ - _DEF_PROTOCOL_BASE] = &UDPMediator::DealOnLineRq;
	//m_protocolMap[_DEF_PROTOCOL_ONLINE_RS - _DEF_PROTOCOL_BASE] = &UDPMediator::DealOnLineRs;
	//m_protocolMap[_DEF_PROTOCOL_OFFLINE_NTF - _DEF_PROTOCOL_BASE] = &UDPMediator::DealOffLineNTF;

}

static ProtocolMap m_MapEnter[] =
{
	{_DEF_PROTOCOL_ONLINE_RQ ,&UDPMediator::DealOnLineRq},
	{_DEF_PROTOCOL_ONLINE_RS ,&UDPMediator::DealOnLineRs},
	{_DEF_PROTOCOL_OFFLINE_NTF,&UDPMediator::DealOffLineNTF},
	{_DEF_PROTOCOL_DATAINFO,&UDPMediator::DealDataInfo},
	{0,0}
};



UDPMediator::~UDPMediator()
{
	  if(m_pudpNet)
	  {
		 
		  delete m_pudpNet;
		  m_pudpNet = nullptr;
	  }
}


bool UDPMediator::open() 
{
	if(!m_pudpNet->InitNetWork())
	{
		return false;
	}
	//向所有人发上线请求
	STRU_ONOFFLINE so;
	so.m_ntype = _DEF_PROTOCOL_ONLINE_RQ;

	if(!m_pudpNet->SendData(INADDR_BROADCAST,(char*)&so,sizeof(so)))
	{
		return false;
	}
	return true;
}
void UDPMediator::close()
{
	STRU_ONOFFLINE so;
	so.m_ntype = _DEF_PROTOCOL_OFFLINE_NTF;
	m_pudpNet->SendData(INADDR_BROADCAST,(char*)&so,sizeof(so));
	
	m_pudpNet->UnInitNetWork();
}

bool UDPMediator::SendData(long lSendIp,char *szbuf,int nlen)
{
	if(!m_pudpNet->SendData(lSendIp,szbuf,nlen))
	{
		return false;
	}
	return true;
}

 void UDPMediator::DealDataInfo(char* szbuf,long lRecvIp)
 {
	 //处理数据
	 STRU_DATAINFO *psd = ( STRU_DATAINFO *)szbuf;
	
	 m_pWnd->SendMessage(UM_DATAMSG,(WPARAM)psd->m_szBuffer,lRecvIp);
 }


void UDPMediator::DealOnLineRq(char* szbuf,long lRecvIp)
{
	//将好友信息放在好友列表中
	m_pWnd->PostMessage(UM_ONLINEMSG,0,lRecvIp);
			
	if(lRecvIp  == INet::GetValidIp())
	{
		return;
	}
	//给好友发回复
	STRU_ONOFFLINE so;
	so.m_ntype = _DEF_PROTOCOL_ONLINE_RS;

	m_pudpNet->SendData(lRecvIp,(char*)&so,sizeof(so));
}
void UDPMediator::DealOnLineRs(char* szbuf,long lRecvIp)
{
	//将好友信息放在好友列表中
	m_pWnd->PostMessage(UM_ONLINEMSG,0,lRecvIp);
}

void UDPMediator::DealOffLineNTF(char* szbuf,long lRecvIp)
{
	//将好友信息放在好友列表中
	m_pWnd->PostMessage(UM_OFFLINEMSG,0,lRecvIp);
}

void UDPMediator::DealData(char* szbuf,long lRecvIp)
{
	//处理数据
	int *ptype =(int*)szbuf;
	int i = 0;
	while(1)
	{
		if(m_MapEnter[i].m_nProtocol == 0)
		{
			break;
		}
		else if(m_MapEnter[i].m_nProtocol == *ptype)
		{
			(this->*m_MapEnter[i].m_pProtocolMap)(szbuf,lRecvIp);
			return;
		}
		i++;
	}
	
}

