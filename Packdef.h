

#ifndef PACKDEF_H
#define PACKDEF_H

#define _DEF_PORT   1234
#define _MAX_BUFFERSIZE  1000

#define _MAX_PROTOCOL_LEN   100

#define _DEF_PACKDEFSIZE   1024
//消息
#define UM_ONLINEMSG        WM_USER + 1
#define UM_OFFLINEMSG       WM_USER + 2
#define UM_DATAMSG          WM_USER + 3
#define UM_DESTROYMSG       WM_USER + 4

#define UM_READMSG          WM_USER + 5
//包类型
#define _DEF_PROTOCOL_BASE       2000
#define _DEF_PROTOCOL_ONLINE_RQ    (_DEF_PROTOCOL_BASE+0) 
#define _DEF_PROTOCOL_ONLINE_RS    (_DEF_PROTOCOL_BASE+1) 

#define _DEF_PROTOCOL_OFFLINE_NTF  (_DEF_PROTOCOL_BASE+2) 

#define _DEF_PROTOCOL_DATAINFO     (_DEF_PROTOCOL_BASE+3) 

#define _DEF_PROTOCOL_FILEINFO     (_DEF_PROTOCOL_BASE+4) 


//协议包
typedef int PackType;
//上线请求、上线回复、下线通知
typedef struct STRU_ONOFFLINE
{
	 PackType m_ntype;
}STRU_ONOFFLINE;


//数据包
typedef struct STRU_DATAINFO
{
	 PackType m_ntype;
	 char     m_szBuffer[_MAX_BUFFERSIZE];
	 int      m_nLen;
}STRU_DATAINFO;






#endif
