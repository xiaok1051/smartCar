/***********************************
 *    fileName:   sc_rmtctl.h
 * discription:   遥控实现 wifi
***********************************/
#ifndef _SC_RMTCTL_H
#define _SC_RMTCTL_H

#define SC_RMTCTL_PORT     2321
#define SC_RMTCTL_BACKLOG  1

#define SC_RMTCTL_MSGMAXLEN 1024

enum enSCRmtCtlMsg
{
	SC_RMTCTL_MSG_REVERSE,
	SC_RMTCTL_MSG_STOP,
	SC_RMTCTL_MSG_FORWARD,
	SC_RMTCTL_MSG_BACKOFF,
	SC_RMTCTL_MSG_TURNLEFT,
	SC_RMTCTL_MSG_TURNRIGHT,
	SC_RMTCTL_MSG_MAX
};

extern int SC_rmtctl_init();

#endif

