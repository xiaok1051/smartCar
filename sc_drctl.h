/***********************************
 *    fileName:   sc_drctl.h
 * discription:   方向控制的头文件
***********************************/
#ifndef _SC_DRCTL_H
#define _SC_DRCTL_H

#define SC_GPIO_EXPORT_FILE      "/sys/class/gpio/export"
#define SC_GPIO_UNEXPORT_FILE    "/sys/class/gpio/unexport"

#define SC_LOW     "low"
#define SC_HIGH    "high"

#define SC_ONE	   "1"
#define SC_ZERO    "0"

#define SC_L298N_ENA    6
#define SC_L298N_IN1    199
#define SC_L298N_IN2    198
#define SC_L298N_ENB    0
#define SC_L298N_IN3    2
#define SC_L298N_IN4    3

extern int SC_drctl_stop();
extern int SC_drctl_forward();
extern int SC_drctl_backoff();
extern int SC_drctl_tuneLeft();
extern int SC_drctl_tuneRight();
extern int SC_drctl_init();
extern int SC_drctl_uninit();

#endif

