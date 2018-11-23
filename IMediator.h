#ifndef IMEDIATOR_H
#define IMEDIATOR_H

#include "Packdef.h"


class IMediator
{
public:
	IMediator(){};
	virtual~IMediator(){};
public:
	virtual bool open() = 0;
	virtual void close() = 0;
	virtual bool SendData(long lSendIp,char *szbuf,int nlen) = 0;
	virtual void DealData(char* szbuf,long lRecvIp) = 0;


};

#endif

