

#ifndef UDPNET_H
#define UDPNET_H

#include "INet.h"
#include "IMediator.h"
#include "MyWnd.h"
#define MAXNUM 64
class UDPNet :public INet
{
public:
	UDPNet(IMediator *pMediator);
	~UDPNet();
public:
	virtual bool InitNetWork() ;  //加载库  创建socket  bind listen 
	virtual void UnInitNetWork() ;
	virtual bool SendData(long lSendIp,char *szbuf,int nlen);
	static   unsigned _stdcall ThreadProc( void * );
	bool   SelectSocket();
private:
	SOCKET m_socklisten;
	SOCKET m_socksend;
	HANDLE m_hThread;
	bool   m_bflagQuit;
	IMediator *m_pMediator;
	//CMyWnd   m_mywnd;
	WSAEVENT m_aryEvent[MAXNUM];
	int    m_nEventNum;
	
};









#endif
