/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210E-EVAL Evaluation Board
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : BAN
*********************************************************************************************************
*/

#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <stddef.h>		   /* Ϊ��ʹNULL��ʶ�� */

#include  <ucos_ii.h>
#include  <app_cfg.h>

#include  "bsp.h"
#include "os_cpu.h"

enum
{
	MSG_BackPic = 0,		
    MSG_WriteNorFlash,
    MSG_SaveSR,
    MSG_BackLight,
	MSG_BackPicPressed
};


/* ����һ�����䣬    ��ֻ��һ������ָ�룬  OSMboxCreate�����ᴴ������������Դ */
extern OS_EVENT *AppUserIFMbox;
extern OS_EVENT *AppMsgMbox;

/* ����һ���ź�����  ��ֻ��һ���ź���ָ�룬OSSEMCreate�����ᴴ������������Դ */
 
extern OS_EVENT *Semp;
extern OS_EVENT *AppBackpicSemp;

/*����һ���ڴ��*/
/*����һ���ڴ���� 2���ڴ�� ÿ���ڴ��2050���ֽ�*/
extern OS_MEM   *CommMem; 
//extern __align(4) INT8U   CommBuf[6][1028];

#endif
