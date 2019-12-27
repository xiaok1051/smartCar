/***********************************
 *    fileName:   sc_rmtctl.c
 * discription:   遥控实现 wifi
***********************************/
/* linux or c repo head file */
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

/* private head file */
#include "sc_pub.h"
#include "sc_rmtctl.h"
#include "sc_drctl.h"

int SC_rmtctl_creatSock()
{
	int iSockFd;
    int iOpt = SO_REUSEADDR;

	iSockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(iSockFd < 0)
	{
		printf("*** Creat socket failed, errno=%d ***\r\n", errno);
	}

    /* set socket option */
    setsockopt(iSockFd, SOL_SOCKET, SO_REUSEADDR, &iOpt, sizeof(iOpt));

	return iSockFd;
}

/* socket bind */
int SC_rmtctl_sockBind(int iSockFd)
{
	int iRet;
	struct sockaddr_in stServAddr;

	memset(&stServAddr, 0, sizeof(stServAddr));
	stServAddr.sin_family = AF_INET;
	stServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stServAddr.sin_port = htons(SC_RMTCTL_PORT);

	iRet = bind(iSockFd, (struct sockaddr *)&stServAddr, sizeof(stServAddr));
	if(ERROR_SUCCESS != iRet)
	{
		printf("*** Bind socket failed, errno=%d!***\r\n", errno);
	}

	return iRet;
}

/* listen */
int SC_rmtctl_listen(int iSockFd)
{
	int iRet;

	iRet = listen(iSockFd, SC_RMTCTL_BACKLOG);
	if(ERROR_SUCCESS != iRet)
	{
		printf("*** Listen failed, errno=%d ***\r\n", errno);
	}

	return iRet;
}

/* accept */
int SC_rmtctl_accept(int iSockFd)
{
	int iCntFd;
	struct sockaddr_in stCltAddr;
    socklen_t addrlen;
	struct timeval stTv;

	addrlen = sizeof(stCltAddr);
	iCntFd = accept(iSockFd, (struct sockaddr *)&stCltAddr, &addrlen);
	if(-1 == iCntFd)
	{
		printf("*** accept failed, errno=%d ***\r\n", errno);
	}
	else
	{
		gettimeofday(&stTv, NULL);
		printf("You got a connection from client's ip %s, port %d at time %ld.%ld\r\n",
			   inet_ntoa(stCltAddr.sin_addr),htons(stCltAddr.sin_port), stTv.tv_sec,stTv.tv_usec);
	}

	return iCntFd;		
}

/* accept */
int SC_rmtctl_recv(int iCntFd, int iBufLen, void *pMsg)
{
	int iRet;

	iRet = recv(iCntFd, pMsg, iBufLen, 0);
	if(0 > iRet)
	{
		printf("*** Recv failed! ***");	
	}
	
	return iRet;
}

/* 消息处理函数 */
int SC_rmtctl_procMsg(int iMsglen, void *pMsg)
{
	int iRet = ERROR_SUCCESS;
	int iMsgType;
	char cMsgType;
	
	memcpy(&cMsgType, pMsg, iMsglen);

	printf("*** cMsgType=%s ***\r\n", &cMsgType);

	switch(cMsgType)
	{
		case '1':
		{
			SC_drctl_stop();
			iRet = ERROR_SUCCESS;
			break;
		}
		case '2':
		{
			SC_drctl_forward();
			iRet = ERROR_SUCCESS;
			break;
		}
		case '3':
		{
			SC_drctl_backoff();
			iRet = ERROR_SUCCESS;
			break;
		}
		default:
		{
			iRet = ERROR_FAILED;
			break;
		}
	}

	return iRet;
}

/* init function 建立监听链接 */
int SC_rmtctl_init()
{
	int iRet;
	int iSockFd;
	int iConSockFd;
	int iRetLen;
	char szMsg[SC_RMTCTL_MSGMAXLEN] = {0};

	iSockFd = SC_rmtctl_creatSock();
	if(0 > iSockFd)
	{
		return ERROR_FAILED;
	}

	iRet = SC_rmtctl_sockBind(iSockFd);
	if(ERROR_SUCCESS != iRet)
	{
		close(iSockFd);
		return ERROR_FAILED;
	}

	iRet = SC_rmtctl_listen(iSockFd);
	if(ERROR_SUCCESS != iRet)
	{
		close(iSockFd);
		return ERROR_FAILED;
	}

	for( ; ; )
	{
		iConSockFd = SC_rmtctl_accept(iSockFd);
		if(0 > iConSockFd)
		{
			close(iSockFd);
			return ERROR_FAILED;
		}

		for( ; ; )
		{
			iRetLen = SC_rmtctl_recv(iConSockFd, SC_RMTCTL_MSGMAXLEN, (void *)szMsg);
			if(0 < iRetLen)
			{
				iRet = SC_rmtctl_procMsg(iRetLen, szMsg);
				if(ERROR_SUCCESS == iRet)
				{
					printf("*** SC rmtctl recv msg %s ***\r\n", szMsg);
				}
			}
			else
			{
				close(iConSockFd);
				break;
			}
		}
	}
}

