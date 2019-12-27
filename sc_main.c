/***********************************
 *    fileName:   main.c
 * discription:   main函数
***********************************/
#include <unistd.h>

#include "sc_pub.h"
#include "sc_drctl.h"
#include "sc_rmtctl.h"

int main()
{
	int iRet;

	SC_drctl_init();
	/*SC_drctl_forward();
	sleep(2);
	SC_drctl_backoff();
	sleep(2);
	SC_drctl_stop();
	SC_drctl_uninit();*/

	SC_rmtctl_init();

	return iRet;
}

