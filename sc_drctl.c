/***********************************
 *    fileName:   sc_drctl.c
 * discription:   提供方向控制函数
***********************************/

/* linux or c head file  */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* private head file  */
#include "sc_pub.h"
#include "sc_drctl.h"

/* export gpio */
int SC_drctl_exportGpio(int iPin)
{
	char szPin[10];
	int iFd;
	int iRet;

	iFd = open(SC_GPIO_EXPORT_FILE, O_WRONLY);
	if(0 > iFd)
	{
		printf("*** Open %s  file failed! ***\r\n", SC_GPIO_EXPORT_FILE);
		return ERROR_FAILED;
	}

	sprintf(szPin, "%d", iPin);
	iRet = write(iFd, szPin, strlen(szPin));
	if(0 > iRet)
    {
		printf("*** Write %s file failed! ***\r\n", SC_GPIO_EXPORT_FILE);
		close(iFd);
		return ERROR_FAILED;
    }

	close(iFd);
	
	return ERROR_SUCCESS;
}

/* unexport gpio */
int SC_drctl_unexportGpio(int iPin)
{
	char szPin[10];
	int iFd;
	int iRet;

	iFd = open(SC_GPIO_UNEXPORT_FILE, O_WRONLY);
	if(0 > iFd)
	{
		printf("*** Open %s  file failed! ***\r\n", SC_GPIO_UNEXPORT_FILE);
		return ERROR_FAILED;
	}

	sprintf(szPin, "%d", iPin);
	iRet = write(iFd, szPin, strlen(szPin));
	if(0 > iRet)
    {
		printf("*** Write %s file failed! ***\r\n", SC_GPIO_UNEXPORT_FILE);
		close(iFd);
		return ERROR_FAILED;
    }

	close(iFd);
	
	return ERROR_SUCCESS;
}

/* 设置gpio的direction */
int SC_drctl_setPinDirection(int iPin, char *szDrc)
{
	char szPath[100] = {0};
	int iFd;
	int iRet;

	sprintf(szPath, "/sys/class/gpio/gpio%d/direction", iPin);
	iFd = open(szPath, O_WRONLY);
	if(0 > iFd)
	{
		printf("*** Open %s  file failed! ***\r\n", szPath);
		return ERROR_FAILED;
	}

	iRet = write(iFd, szDrc, strlen(szDrc));
	if(0 > iRet)
	{
		printf("*** Write %s file failed! ***\r\n", szPath);
		close(iFd);
		return ERROR_FAILED;
	}
	
	close(iFd);
	
	return ERROR_SUCCESS;
}

/* 设置gpio的Value */
int SC_drctl_setPinValue(int iPin, char *szValue)
{
	char szPath[100] = {0};
	int iFd;
	int iRet;

	sprintf(szPath, "/sys/class/gpio/gpio%d/value", iPin);
	iFd = open(szPath, O_WRONLY);
	if(0 > iFd)
	{
		printf("*** Open %s  file failed! ***\r\n", szPath);
		return ERROR_FAILED;
	}

	iRet = write(iFd, szValue, strlen(szValue));
	if(0 > iRet)
	{
		printf("*** Write %s file failed! ***\r\n", szPath);
		close(iFd);
		return ERROR_FAILED;
	}
	
	close(iFd);

	return ERROR_SUCCESS;
}

/* 停止 */
int SC_drctl_stop()
{
	int iRet;

	iRet  = SC_drctl_setPinValue(SC_L298N_ENA, SC_ZERO);
	iRet |= SC_drctl_setPinValue(SC_L298N_IN1, SC_ZERO);
	iRet |= SC_drctl_setPinValue(SC_L298N_IN2, SC_ZERO);
	iRet |= SC_drctl_setPinValue(SC_L298N_ENB, SC_ZERO);
	iRet |= SC_drctl_setPinValue(SC_L298N_IN3, SC_ZERO);
	iRet |= SC_drctl_setPinValue(SC_L298N_IN4, SC_ZERO);

	if(ERROR_SUCCESS != iRet)
	{
		printf("*** Set car stop failed ***\r\n");
	}
	
	return iRet;
}

/* 前进 */
int SC_drctl_forward()
{
	int iRet;

	iRet  = SC_drctl_setPinValue(SC_L298N_IN1, SC_ONE);
	iRet |= SC_drctl_setPinValue(SC_L298N_IN2, SC_ZERO);
	iRet |= SC_drctl_setPinValue(SC_L298N_IN3, SC_ONE);
	iRet |= SC_drctl_setPinValue(SC_L298N_IN4, SC_ZERO);

	iRet |= SC_drctl_setPinValue(SC_L298N_ENA, SC_ONE);
	iRet |= SC_drctl_setPinValue(SC_L298N_ENB, SC_ONE);

	if(ERROR_SUCCESS != iRet)
	{
		printf("*** Set the car forward failed! ***\r\n");
	}

	return iRet;
}

/* 后退 */
int SC_drctl_backoff()
{
	int iRet;

	iRet  = SC_drctl_setPinValue(SC_L298N_IN1, SC_ZERO);
	iRet |= SC_drctl_setPinValue(SC_L298N_IN2, SC_ONE);
	iRet |= SC_drctl_setPinValue(SC_L298N_IN3, SC_ZERO);
	iRet |= SC_drctl_setPinValue(SC_L298N_IN4, SC_ONE);

	iRet |= SC_drctl_setPinValue(SC_L298N_ENA, SC_ONE);
	iRet |= SC_drctl_setPinValue(SC_L298N_ENB, SC_ONE);

	if(ERROR_SUCCESS != iRet)
	{
		printf("*** Set the car forward failed! ***\r\n");
	}

	return iRet;
}

/* 左转 */
int SC_drctl_tuneLeft()
{
	int iRet = ERROR_FAILED;
	return iRet;
}

/* 右转 */
int SC_drctl_tuneRight()
{
	int iRet = ERROR_FAILED;
	return iRet;
}

/* int SC_drctl_init  */
int SC_drctl_init()
{
    int iRet;

	/* 导出引脚 */
	iRet  = SC_drctl_exportGpio(SC_L298N_ENA);
	iRet |= SC_drctl_exportGpio(SC_L298N_IN1);
	iRet |= SC_drctl_exportGpio(SC_L298N_IN2);
	iRet |= SC_drctl_exportGpio(SC_L298N_ENB);
	iRet |= SC_drctl_exportGpio(SC_L298N_IN3);
	iRet |= SC_drctl_exportGpio(SC_L298N_IN4);

	/* 初始所有引脚都设置为output，并且输出低电平 */
	iRet |= SC_drctl_setPinDirection(SC_L298N_ENA, SC_LOW);
	iRet |= SC_drctl_setPinDirection(SC_L298N_IN1, SC_LOW);
	iRet |= SC_drctl_setPinDirection(SC_L298N_IN2, SC_LOW);
	iRet |= SC_drctl_setPinDirection(SC_L298N_ENB, SC_LOW);
	iRet |= SC_drctl_setPinDirection(SC_L298N_IN3, SC_LOW);
	iRet |= SC_drctl_setPinDirection(SC_L298N_IN4, SC_LOW);

	if(ERROR_SUCCESS != iRet)
	{
		printf("*** GPIO init failed ***\r\n");
	}

	return iRet;
}

/* int SC_drctl_uninit  */
int SC_drctl_uninit()
{
	int iRet;

	iRet  = SC_drctl_unexportGpio(SC_L298N_IN1);
	iRet |= SC_drctl_unexportGpio(SC_L298N_IN2);
	iRet |= SC_drctl_unexportGpio(SC_L298N_IN3);
	iRet |= SC_drctl_unexportGpio(SC_L298N_IN4);
	iRet |= SC_drctl_unexportGpio(SC_L298N_ENA);
	iRet |= SC_drctl_unexportGpio(SC_L298N_ENB);

	return iRet;
}

