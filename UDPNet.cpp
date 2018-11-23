#include "stdafx.h"
#include "UDPNet.h"

UDPNet::UDPNet(IMediator *pMediator)
{
	m_socklisten = NULL;
	m_socksend = NULL;
	m_hThread = NULL;
	m_bflagQuit = true;
	m_pMediator = pMediator;
	m_nEventNum = 0;
}

UDPNet::~UDPNet()
{}

bool UDPNet::InitNetWork() 
{
	
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;


    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
	{
        return false;
    }


    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
     
        
        UnInitNetWork();
        return false;
    }
   
	m_socklisten = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	m_socksend = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(m_socklisten ==  INVALID_SOCKET ||m_socksend ==  INVALID_SOCKET  )
	{
		UnInitNetWork();
        return false;
	}
	sockaddr_in  addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_DEF_PORT);
	addr.sin_addr.s_addr = GetValidIp();
	if(bind(m_socklisten,(const sockaddr*)&addr,sizeof(addr)) == SOCKET_ERROR)
	{
		UnInitNetWork();
        return false;
	}
	//m_socksend  设置广播
	BOOL bval= TRUE;
	setsockopt(m_socksend,SOL_SOCKET,SO_BROADCAST,(const char*)&bval,sizeof(BOOL));

	//向windows注册
	
	WSAEVENT we = WSACreateEvent(); //创建人工事件
	WSAEventSelect(m_socklisten,we,FD_READ);
	m_aryEvent[m_nEventNum++] = we;
	

	m_hThread = (HANDLE) _beginthreadex(nullptr,0,&ThreadProc,this,0,0);


	//创建窗口
	/*if(!CMyWnd::GetMyWnd()->Create(NULL,_T("MyWnd")))
	{
		UnInitNetWork();
        return false;
	}
	CMyWnd::GetMyWnd()->SetMediator(m_pMediator);*/

	//向windows注册
	//WSAAsyncSelect(m_socklisten,CMyWnd::GetMyWnd()->m_hWnd,UM_READMSG,FD_READ);


	
	//设置非阻塞
	//u_long argp = true;
	//ioctlsocket(m_socklisten,FIONBIO,&argp);


	//recvfrom-----创建线程
	//m_hThread = (HANDLE) _beginthreadex(nullptr,0,&ThreadProc,this,0,0);


    return true;
}

unsigned _stdcall UDPNet::ThreadProc( void * lpvoid)
{
	UDPNet *pthis = (UDPNet *)lpvoid;
	char szbuf[_DEF_PACKDEFSIZE] = {0};
	sockaddr_in addrClient;
	int nClientSize = sizeof(sockaddr_in);
	int nRelReadNum;
	WSANETWORKEVENTS wwe;
	while(pthis->m_bflagQuit)
	{
		/*if(!pthis->SelectSocket())
		{
			continue;
		}*/
		int nindex = WSAWaitForMultipleEvents(pthis->m_nEventNum,pthis->m_aryEvent, FALSE,WSA_INFINITE,TRUE);

		nindex -= WSA_WAIT_EVENT_0;
		WSAEnumNetworkEvents(pthis->m_socklisten,pthis->m_aryEvent[nindex],&wwe);
		//ResetEvent(pthis->m_we);
		if(wwe.lNetworkEvents == FD_READ)
		{
			nRelReadNum = recvfrom(pthis->m_socklisten,szbuf,_DEF_PACKDEFSIZE,
			                   0,(sockaddr*)&addrClient,&nClientSize);

			if(nRelReadNum >0)
			{
				//处理
				pthis->m_pMediator->DealData(szbuf,addrClient.sin_addr.s_addr);
			}
		}
		
	}

	return 0;
}

bool UDPNet::SelectSocket()
{
	timeval tv;
	tv.tv_sec =0;
	tv.tv_usec = 100;
	fd_set fdsets;
	FD_ZERO(&fdsets);
	
	FD_SET(m_socklisten,&fdsets);

	select(NULL,&fdsets,NULL,NULL,&tv);

	if(!FD_ISSET(m_socklisten,&fdsets))
	{
		return false;
	}
	return true;
}

void UDPNet::UnInitNetWork()
{
	WSACleanup();

	if(m_socklisten)
	{
		closesocket(m_socklisten);
		m_socklisten = NULL;
	}
	if(m_socksend)
	{
		closesocket(m_socksend);
		m_socksend = NULL;
	}

	m_bflagQuit = false;
	if(m_hThread)
	{
		if(WAIT_TIMEOUT == WaitForSingleObject(m_hThread,100))
		{
			TerminateThread(m_hThread,-1);
		}
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	
	/*if(m_we)
	{
		WSACloseEvent(m_we);
		m_we = NULL;
	}*/
	CMyWnd::DeleteMyWnd();
}
bool UDPNet::SendData(long lSendIp,char *szbuf,int nlen)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_DEF_PORT);
	addr.sin_addr.S_un.S_addr = lSendIp;
	if(sendto(m_socksend,szbuf,nlen,0,(const sockaddr*)&addr,sizeof(addr))<=0)
	{
		return false;
	}
	return true;
}
