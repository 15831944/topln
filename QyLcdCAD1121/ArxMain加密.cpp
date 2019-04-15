//************************************************/
/*       软件名称：LcdCAD                        */
/*       文件名称：ArxMain.cpp                   */
/*       程序功能：液晶显示器图形处理软件        */
/*       相关文件：ArxMain.def                   */
/*       运行命令：见说明                        */
/*       运行环境：AutoCAD 2000                  */
/*       设计人员：徐凌飞  林进红                */
/*************************************************/


#include <rxregsvc.h>
#include <acedads.h>
#include <rxdefs.h>
#include <adslib.h>
#include <conio.h>

#include "StdAfx.h"
#include "StdArx.h"
#include "resource.h"
#include <afxdllx.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <aced.h>
#include <dbents.h>
#include <dbsymtb.h>
#include <dbapserv.h>
#include <geassign.h>
#include <adscodes.h>
#include <dbcurve.h>
#include <dbpl.h>
#include <process.h>
#include <vfw.h>
#include <dbhatch.h>
#include <dbregion.h>
#include <dbgroup.h>
#include <dbray.h>
#include <dbmline.h>
#include <dbxline.h>
#include <dbelipse.h>
#include <dbspline.h>
#include <acdocman.h>
#include <acutmem.h>
#include <iostream.h>
#include <dblead.h>



/*********************************************/
/*                                           */
/*              加密部分                     */
/*                                           */
/*********************************************/

//#include "windows.h"
#include "D:\senseIII\vc\AutoC011.h"
#include "D:\senseIII\vc\sense3.h"

#define	InAddr	33
#define	ReadMemAddr	1
#define	DecAddr	3
#define	EncAddr	16
#define	WriMemAddr	29
#define MinData  1E-6  //是小数为0.000001
#define MaxData  6E+7  //是大数为60000000

SENSE3DATA ssd;
unsigned short dataH;
unsigned short dataL;
unsigned short NoDog1 = 0;
unsigned short NoDog2 = 0;
unsigned short NoDog3 = 0;
unsigned short NoDog4 = 0;
unsigned short CaseFlag;

unsigned short OpenSense3(void)
{
	ssd.Password[0] = 755;
	ssd.Password[1] = -6638;
	ssd.Password[2] = 794;
	ssd.FunctionCode = Sense3Init;
	SENSE3(&ssd);
	return ssd.ReturnFlag;
}

void CloseSense3(void)
{
	ssd.FunctionCode = Sense3Exit;
	SENSE3(&ssd);
}

unsigned short ReadMemory0(Sense3AutoData p)
{
	unsigned short v0;
	unsigned short i;
	*(p+6)=*(p+25);
	*(p+12)=*(p+17);
	*(p+19)=*(p+Read_Addr);
	*(p+16)=*(p+12)^*(p+19);
	*(p+9)=*(p+6)+*(p+16);
	for (i=0;i<7;i++)
		*(p+9)=(*(p+9)<<1)+(*(p+9)>>15);
	*(p+18)=*(p+19)+*(p+16);
	*(p+22)=*(p+18)^*(p+16);
	*(p+23)=*(p+9)+*(p+22);
	*(p+7)=*(p+18)-*(p+22);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+23);
	ssd.OutBuff[2]=*(p+7);
	ssd.OutBuff[1]=ReadMemAddr;
	ssd.OutBuff[0]=*(p+22);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+1)=ssd.InBuff[3];
	*(p+0)=ssd.InBuff[2];
	*(p+2)=ssd.InBuff[0];
	*(p+13)=*(p+2)^*(p+0);
	*(p+6)=*(p+1)+*(p+13);
	for (i=0;i<7;i++)
		*(p+6)=(*(p+6)<<1)+(*(p+6)>>15);
	*(p+15)=*(p+0)+*(p+13);
	*(p+23)=*(p+15)^*(p+13);
	*(p+11)=*(p+6)+*(p+23);
	*(p+16)=*(p+15)-*(p+23);
	v0=*(p+23);
	*(p+15)=*(p+11);
	*(p+Read_Data)=v0;
	*(p+11)=*(p+16);
	return ssd.ReturnFlag;
}

unsigned short ReadMemory1(Sense3AutoData p)
{
	unsigned short v0;
	unsigned short i;
	*(p+Read_Data)=*(p+Read_Addr);
	*(p+13)=*(p+9);
	*(p+12)=*(p+24);
	*(p+5)=*(p+13)^*(p+0);
	*(p+7)=*(p+12)+*(p+5);
	for (i=0;i<7;i++)
		*(p+7)=(*(p+7)<<1)+(*(p+7)>>15);
	*(p+6)=*(p+0)+*(p+5);
	*(p+11)=*(p+6)^*(p+5);
	*(p+14)=*(p+7)+*(p+11);
	*(p+15)=*(p+6)-*(p+11);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+14);
	ssd.OutBuff[2]=*(p+15);
	ssd.OutBuff[1]=ReadMemAddr;
	ssd.OutBuff[0]=*(p+11);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+3)=ssd.InBuff[3];
	*(p+20)=ssd.InBuff[2];
	*(p+8)=ssd.InBuff[0];
	*(p+6)=*(p+8)^*(p+20);
	*(p+11)=*(p+3)+*(p+6);
	for (i=0;i<7;i++)
		*(p+11)=(*(p+11)<<1)+(*(p+11)>>15);
	*(p+9)=*(p+20)+*(p+6);
	*(p+20)=*(p+9)^*(p+6);
	*(p+14)=*(p+11)+*(p+20);
	*(p+2)=*(p+9)-*(p+20);
	v0=*(p+20);
	*(p+14)=*(p+14);
	*(p+Read_Data)=v0;
	*(p+20)=*(p+2);
	return ssd.ReturnFlag;
}

unsigned short ReadMemory2(Sense3AutoData p)
{
	unsigned short v0;
	unsigned short i;
	*(p+25)=*(p+Read_Addr);
	*(p+1)=*(p+20);
	*(p+21)=*(p+6);
	*(p+19)=*(p+21)^*(p+25);
	*(p+3)=*(p+1)+*(p+19);
	for (i=0;i<7;i++)
		*(p+3)=(*(p+3)<<1)+(*(p+3)>>15);
	*(p+22)=*(p+25)+*(p+19);
	*(p+5)=*(p+22)^*(p+19);
	*(p+10)=*(p+3)+*(p+5);
	*(p+13)=*(p+22)-*(p+5);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+10);
	ssd.OutBuff[2]=*(p+13);
	ssd.OutBuff[1]=ReadMemAddr;
	ssd.OutBuff[0]=*(p+5);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+17)=ssd.InBuff[3];
	*(p+1)=ssd.InBuff[2];
	*(p+10)=ssd.InBuff[0];
	*(p+22)=*(p+10)^*(p+1);
	*(p+23)=*(p+17)+*(p+22);
	for (i=0;i<7;i++)
		*(p+23)=(*(p+23)<<1)+(*(p+23)>>15);
	*(p+9)=*(p+1)+*(p+22);
	*(p+14)=*(p+9)^*(p+22);
	*(p+7)=*(p+23)+*(p+14);
	*(p+4)=*(p+9)-*(p+14);
	v0=*(p+14);
	*(p+11)=*(p+7);
	*(p+Read_Data)=v0;
	*(p+9)=*(p+4);
	return ssd.ReturnFlag;
}

unsigned short Decrypt0(Sense3AutoData p)
{
	unsigned short v0,v1;
	unsigned short i;
	*(p+13)=*(p+10);
	*(p+4)=*(p+Decrypt_Highword_Data);
	*(p+7)=*(p+Decrypt_Lowword_Data);
	*(p+14)=*(p+4)^*(p+7);
	*(p+10)=*(p+13)+*(p+14);
	for (i=0;i<7;i++)
		*(p+10)=(*(p+10)<<1)+(*(p+10)>>15);
	*(p+17)=*(p+7)+*(p+14);
	*(p+22)=*(p+17)^*(p+14);
	*(p+15)=*(p+10)+*(p+22);
	*(p+16)=*(p+17)-*(p+22);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+15);
	ssd.OutBuff[2]=*(p+16);
	ssd.OutBuff[1]=DecAddr;
	ssd.OutBuff[0]=*(p+22);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+13)=ssd.InBuff[3];
	*(p+12)=ssd.InBuff[2];
	*(p+4)=ssd.InBuff[0];
	*(p+2)=*(p+4)^*(p+12);
	*(p+22)=*(p+13)+*(p+2);
	for (i=0;i<7;i++)
		*(p+22)=(*(p+22)<<1)+(*(p+22)>>15);
	*(p+23)=*(p+12)+*(p+2);
	*(p+13)=*(p+23)^*(p+2);
	*(p+15)=*(p+22)+*(p+13);
	*(p+18)=*(p+23)-*(p+13);
	v0=*(p+13);
	v1=*(p+18);
	*(p+13)=*(p+15);
	*(p+Decrypt_Highword_Data)=v0;
	*(p+Decrypt_Lowword_Data)=v1;
	return ssd.ReturnFlag;
}

unsigned short Decrypt1(Sense3AutoData p)
{
	unsigned short v0,v1;
	unsigned short i;
	*(p+14)=*(p+Decrypt_Highword_Data);
	*(p+9)=*(p+Decrypt_Lowword_Data);
	*(p+25)=*(p+12);
	*(p+0)=*(p+14)^*(p+9);
	*(p+23)=*(p+25)+*(p+0);
	for (i=0;i<7;i++)
		*(p+23)=(*(p+23)<<1)+(*(p+23)>>15);
	*(p+3)=*(p+9)+*(p+0);
	*(p+20)=*(p+3)^*(p+0);
	*(p+13)=*(p+23)+*(p+20);
	*(p+21)=*(p+3)-*(p+20);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+13);
	ssd.OutBuff[2]=*(p+21);
	ssd.OutBuff[1]=DecAddr;
	ssd.OutBuff[0]=*(p+20);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+16)=ssd.InBuff[3];
	*(p+12)=ssd.InBuff[2];
	*(p+18)=ssd.InBuff[0];
	*(p+25)=*(p+18)^*(p+12);
	*(p+22)=*(p+16)+*(p+25);
	for (i=0;i<7;i++)
		*(p+22)=(*(p+22)<<1)+(*(p+22)>>15);
	*(p+24)=*(p+12)+*(p+25);
	*(p+2)=*(p+24)^*(p+25);
	*(p+11)=*(p+22)+*(p+2);
	*(p+21)=*(p+24)-*(p+2);
	v0=*(p+2);
	v1=*(p+21);
	*(p+20)=*(p+11);
	*(p+Decrypt_Highword_Data)=v0;
	*(p+Decrypt_Lowword_Data)=v1;
	return ssd.ReturnFlag;
}

unsigned short Decrypt2(Sense3AutoData p)
{
	unsigned short v0,v1;
	unsigned short i;
	*(p+18)=*(p+Decrypt_Highword_Data);
	*(p+7)=*(p+Decrypt_Lowword_Data);
	*(p+21)=*(p+14);
	*(p+10)=*(p+18)^*(p+7);
	*(p+3)=*(p+21)+*(p+10);
	for (i=0;i<7;i++)
		*(p+3)=(*(p+3)<<1)+(*(p+3)>>15);
	*(p+23)=*(p+7)+*(p+10);
	*(p+25)=*(p+23)^*(p+10);
	*(p+22)=*(p+3)+*(p+25);
	*(p+7)=*(p+23)-*(p+25);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+22);
	ssd.OutBuff[2]=*(p+7);
	ssd.OutBuff[1]=DecAddr;
	ssd.OutBuff[0]=*(p+25);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+24)=ssd.InBuff[3];
	*(p+2)=ssd.InBuff[2];
	*(p+15)=ssd.InBuff[0];
	*(p+6)=*(p+15)^*(p+2);
	*(p+1)=*(p+24)+*(p+6);
	for (i=0;i<7;i++)
		*(p+1)=(*(p+1)<<1)+(*(p+1)>>15);
	*(p+4)=*(p+2)+*(p+6);
	*(p+8)=*(p+4)^*(p+6);
	*(p+24)=*(p+1)+*(p+8);
	*(p+16)=*(p+4)-*(p+8);
	v0=*(p+8);
	v1=*(p+16);
	*(p+8)=*(p+24);
	*(p+Decrypt_Highword_Data)=v0;
	*(p+Decrypt_Lowword_Data)=v1;
	return ssd.ReturnFlag;
}

unsigned short Decrypt3(Sense3AutoData p)
{
	unsigned short v0,v1;
	unsigned short i;
	*(p+17)=*(p+Decrypt_Highword_Data);
	*(p+8)=*(p+Decrypt_Lowword_Data);
	*(p+6)=*(p+11);
	*(p+3)=*(p+17)^*(p+8);
	*(p+22)=*(p+6)+*(p+3);
	for (i=0;i<7;i++)
		*(p+22)=(*(p+22)<<1)+(*(p+22)>>15);
	*(p+21)=*(p+8)+*(p+3);
	*(p+14)=*(p+21)^*(p+3);
	*(p+0)=*(p+22)+*(p+14);
	*(p+5)=*(p+21)-*(p+14);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+0);
	ssd.OutBuff[2]=*(p+5);
	ssd.OutBuff[1]=DecAddr;
	ssd.OutBuff[0]=*(p+14);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+10)=ssd.InBuff[3];
	*(p+1)=ssd.InBuff[2];
	*(p+24)=ssd.InBuff[0];
	*(p+20)=*(p+24)^*(p+1);
	*(p+18)=*(p+10)+*(p+20);
	for (i=0;i<7;i++)
		*(p+18)=(*(p+18)<<1)+(*(p+18)>>15);
	*(p+15)=*(p+1)+*(p+20);
	*(p+23)=*(p+15)^*(p+20);
	*(p+9)=*(p+18)+*(p+23);
	*(p+6)=*(p+15)-*(p+23);
	v0=*(p+23);
	v1=*(p+6);
	*(p+23)=*(p+9);
	*(p+Decrypt_Highword_Data)=v0;
	*(p+Decrypt_Lowword_Data)=v1;
	return ssd.ReturnFlag;
}

unsigned short Decrypt4(Sense3AutoData p)
{
	unsigned short v0,v1;
	unsigned short i;
	*(p+21)=*(p+Decrypt_Highword_Data);
	*(p+12)=*(p+Decrypt_Lowword_Data);
	*(p+16)=*(p+11);
	*(p+6)=*(p+21)^*(p+12);
	*(p+20)=*(p+16)+*(p+6);
	for (i=0;i<7;i++)
		*(p+20)=(*(p+20)<<1)+(*(p+20)>>15);
	*(p+24)=*(p+12)+*(p+6);
	*(p+21)=*(p+24)^*(p+6);
	*(p+9)=*(p+20)+*(p+21);
	*(p+10)=*(p+24)-*(p+21);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+9);
	ssd.OutBuff[2]=*(p+10);
	ssd.OutBuff[1]=DecAddr;
	ssd.OutBuff[0]=*(p+21);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+15)=ssd.InBuff[3];
	*(p+11)=ssd.InBuff[2];
	*(p+19)=ssd.InBuff[0];
	*(p+8)=*(p+19)^*(p+11);
	*(p+22)=*(p+15)+*(p+8);
	for (i=0;i<7;i++)
		*(p+22)=(*(p+22)<<1)+(*(p+22)>>15);
	*(p+20)=*(p+11)+*(p+8);
	*(p+23)=*(p+20)^*(p+8);
	*(p+11)=*(p+22)+*(p+23);
	*(p+16)=*(p+20)-*(p+23);
	v0=*(p+23);
	v1=*(p+16);
	*(p+6)=*(p+11);
	*(p+Decrypt_Highword_Data)=v0;
	*(p+Decrypt_Lowword_Data)=v1;
	return ssd.ReturnFlag;
}

void DecryptBak(Sense3AutoData p)
{
	unsigned short v0,v1;
	unsigned short i;
	*(p+16)=*(p+DecryptBak_Lowword_Data)-*(p+DecryptBak_Highword_Data);
	for (i=0;i<4;i++)
		*(p+DecryptBak_Highword_Data)=(*(p+DecryptBak_Highword_Data)<<1)+(*(p+DecryptBak_Highword_Data)>>15);
	*(p+11)=*(p+DecryptBak_Highword_Data)-*(p+16);
	*(p+DecryptBak_Lowword_Data)=*(p+16)+*(p+11);
	*(p+3)=*(p+11)-*(p+DecryptBak_Lowword_Data);
	*(p+11)=*(p+3)-*(p+DecryptBak_Lowword_Data);
	for (i=0;i<5;i++)
		*(p+DecryptBak_Lowword_Data)=(*(p+DecryptBak_Lowword_Data)<<1)+(*(p+DecryptBak_Lowword_Data)>>15);
	*(p+8)=*(p+DecryptBak_Lowword_Data)-*(p+11);
	v0=*(p+11);
	v1=*(p+8);
	*(p+DecryptBak_Highword_Data)=v0;
	*(p+DecryptBak_Lowword_Data)=v1;
}

unsigned short Encrypt0(Sense3AutoData p)
{
	unsigned short v0,v1;
	unsigned short i;
	*(p+12)=*(p+Encrypt_Highword_Data);
	*(p+4)=*(p+Encrypt_Lowword_Data);
	*(p+10)=*(p+20);
	*(p+24)=*(p+12)^*(p+4);
	*(p+23)=*(p+10)+*(p+24);
	for (i=0;i<7;i++)
		*(p+23)=(*(p+23)<<1)+(*(p+23)>>15);
	*(p+2)=*(p+4)+*(p+24);
	*(p+1)=*(p+2)^*(p+24);
	*(p+12)=*(p+23)+*(p+1);
	*(p+6)=*(p+2)-*(p+1);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+12);
	ssd.OutBuff[2]=*(p+6);
	ssd.OutBuff[1]=EncAddr;
	ssd.OutBuff[0]=*(p+1);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+11)=ssd.InBuff[3];
	*(p+13)=ssd.InBuff[2];
	*(p+23)=ssd.InBuff[0];
	*(p+5)=*(p+23)^*(p+13);
	*(p+19)=*(p+11)+*(p+5);
	for (i=0;i<7;i++)
		*(p+19)=(*(p+19)<<1)+(*(p+19)>>15);
	*(p+1)=*(p+13)+*(p+5);
	*(p+15)=*(p+1)^*(p+5);
	*(p+6)=*(p+19)+*(p+15);
	*(p+17)=*(p+1)-*(p+15);
	v0=*(p+15);
	v1=*(p+17);
	*(p+17)=*(p+6);
	*(p+Encrypt_Highword_Data)=v0;
	*(p+Encrypt_Lowword_Data)=v1;
	return ssd.ReturnFlag;
}

unsigned short Encrypt1(Sense3AutoData p)
{
	unsigned short v0,v1;
	unsigned short i;
	*(p+19)=*(p+Encrypt_Highword_Data);
	*(p+4)=*(p+Encrypt_Lowword_Data);
	*(p+8)=*(p+25);
	*(p+23)=*(p+19)^*(p+4);
	*(p+2)=*(p+8)+*(p+23);
	for (i=0;i<7;i++)
		*(p+2)=(*(p+2)<<1)+(*(p+2)>>15);
	*(p+12)=*(p+4)+*(p+23);
	*(p+13)=*(p+12)^*(p+23);
	*(p+21)=*(p+2)+*(p+13);
	*(p+24)=*(p+12)-*(p+13);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+21);
	ssd.OutBuff[2]=*(p+24);
	ssd.OutBuff[1]=EncAddr;
	ssd.OutBuff[0]=*(p+13);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+3)=ssd.InBuff[3];
	*(p+5)=ssd.InBuff[2];
	*(p+20)=ssd.InBuff[0];
	*(p+1)=*(p+20)^*(p+5);
	*(p+4)=*(p+3)+*(p+1);
	for (i=0;i<7;i++)
		*(p+4)=(*(p+4)<<1)+(*(p+4)>>15);
	*(p+17)=*(p+5)+*(p+1);
	*(p+19)=*(p+17)^*(p+1);
	*(p+20)=*(p+4)+*(p+19);
	*(p+23)=*(p+17)-*(p+19);
	v0=*(p+19);
	v1=*(p+23);
	*(p+6)=*(p+20);
	*(p+Encrypt_Highword_Data)=v0;
	*(p+Encrypt_Lowword_Data)=v1;
	return ssd.ReturnFlag;
}

unsigned short Encrypt2(Sense3AutoData p)
{
	unsigned short v0,v1;
	unsigned short i;
	*(p+7)=*(p+Encrypt_Highword_Data);
	*(p+22)=*(p+Encrypt_Lowword_Data);
	*(p+4)=*(p+10);
	*(p+6)=*(p+7)^*(p+22);
	*(p+1)=*(p+4)+*(p+6);
	for (i=0;i<7;i++)
		*(p+1)=(*(p+1)<<1)+(*(p+1)>>15);
	*(p+15)=*(p+22)+*(p+6);
	*(p+10)=*(p+15)^*(p+6);
	*(p+23)=*(p+1)+*(p+10);
	*(p+24)=*(p+15)-*(p+10);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+23);
	ssd.OutBuff[2]=*(p+24);
	ssd.OutBuff[1]=EncAddr;
	ssd.OutBuff[0]=*(p+10);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+20)=ssd.InBuff[3];
	*(p+19)=ssd.InBuff[2];
	*(p+15)=ssd.InBuff[0];
	*(p+10)=*(p+15)^*(p+19);
	*(p+18)=*(p+20)+*(p+10);
	for (i=0;i<7;i++)
		*(p+18)=(*(p+18)<<1)+(*(p+18)>>15);
	*(p+9)=*(p+19)+*(p+10);
	*(p+3)=*(p+9)^*(p+10);
	*(p+4)=*(p+18)+*(p+3);
	*(p+8)=*(p+9)-*(p+3);
	v0=*(p+3);
	v1=*(p+8);
	*(p+5)=*(p+4);
	*(p+Encrypt_Highword_Data)=v0;
	*(p+Encrypt_Lowword_Data)=v1;
	return ssd.ReturnFlag;
}

void EncryptBak(Sense3AutoData p)
{
	unsigned short v0,v1;
	unsigned short i;
	*(p+23)=*(p+EncryptBak_Lowword_Data)+*(p+EncryptBak_Highword_Data);
	for (i=0;i<5;i++)
		*(p+23)=(*(p+23)>>1)+(*(p+23)<<15);
	*(p+13)=*(p+EncryptBak_Highword_Data)+*(p+23);
	*(p+0)=*(p+13)+*(p+23);
	*(p+EncryptBak_Lowword_Data)=*(p+23)-*(p+0);
	*(p+EncryptBak_Highword_Data)=*(p+0)+*(p+EncryptBak_Lowword_Data);
	for (i=0;i<4;i++)
		*(p+EncryptBak_Highword_Data)=(*(p+EncryptBak_Highword_Data)>>1)+(*(p+EncryptBak_Highword_Data)<<15);
	*(p+1)=*(p+EncryptBak_Lowword_Data)+*(p+EncryptBak_Highword_Data);
	v0=*(p+EncryptBak_Highword_Data);
	v1=*(p+1);
	*(p+EncryptBak_Highword_Data)=v0;
	*(p+EncryptBak_Lowword_Data)=v1;
}

unsigned short WriteMemory0(Sense3AutoData p)
{
	unsigned short v0;
	unsigned short i;
	*(p+9)=*(p+4);
	*(p+23)=*(p+Write_Addr);
	*(p+5)=*(p+Write_Data);
	*(p+16)=*(p+23)^*(p+5);
	*(p+24)=*(p+9)+*(p+16);
	for (i=0;i<7;i++)
		*(p+24)=(*(p+24)<<1)+(*(p+24)>>15);
	*(p+19)=*(p+5)+*(p+16);
	*(p+15)=*(p+19)^*(p+16);
	*(p+23)=*(p+24)+*(p+15);
	*(p+7)=*(p+19)-*(p+15);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+23);
	ssd.OutBuff[2]=*(p+7);
	ssd.OutBuff[1]=WriMemAddr;
	ssd.OutBuff[0]=*(p+15);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+9)=ssd.InBuff[3];
	*(p+17)=ssd.InBuff[2];
	*(p+22)=ssd.InBuff[0];
	*(p+14)=*(p+22)^*(p+17);
	*(p+5)=*(p+9)+*(p+14);
	for (i=0;i<7;i++)
		*(p+5)=(*(p+5)<<1)+(*(p+5)>>15);
	*(p+8)=*(p+17)+*(p+14);
	*(p+21)=*(p+8)^*(p+14);
	*(p+19)=*(p+5)+*(p+21);
	*(p+2)=*(p+8)-*(p+21);
	v0=*(p+2);
	*(p+21)=*(p+19);
	*(p+5)=*(p+21);
	*(p+Write_Data)=v0;
	return ssd.ReturnFlag;
}

unsigned short WriteMemory1(Sense3AutoData p)
{
	unsigned short v0;
	unsigned short i;
	*(p+18)=*(p+Write_Addr);
	*(p+12)=*(p+Write_Data);
	*(p+17)=*(p+15);
	*(p+0)=*(p+18)^*(p+12);
	*(p+9)=*(p+17)+*(p+0);
	for (i=0;i<7;i++)
		*(p+9)=(*(p+9)<<1)+(*(p+9)>>15);
	*(p+21)=*(p+12)+*(p+0);
	*(p+18)=*(p+21)^*(p+0);
	*(p+16)=*(p+9)+*(p+18);
	*(p+5)=*(p+21)-*(p+18);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+16);
	ssd.OutBuff[2]=*(p+5);
	ssd.OutBuff[1]=WriMemAddr;
	ssd.OutBuff[0]=*(p+18);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+7)=ssd.InBuff[3];
	*(p+4)=ssd.InBuff[2];
	*(p+11)=ssd.InBuff[0];
	*(p+1)=*(p+11)^*(p+4);
	*(p+12)=*(p+7)+*(p+1);
	for (i=0;i<7;i++)
		*(p+12)=(*(p+12)<<1)+(*(p+12)>>15);
	*(p+15)=*(p+4)+*(p+1);
	*(p+9)=*(p+15)^*(p+1);
	*(p+23)=*(p+12)+*(p+9);
	*(p+16)=*(p+15)-*(p+9);
	v0=*(p+16);
	*(p+18)=*(p+23);
	*(p+19)=*(p+9);
	*(p+Write_Data)=v0;
	return ssd.ReturnFlag;
}

unsigned short WriteMemory2(Sense3AutoData p)
{
	unsigned short v0;
	unsigned short i;
	*(p+10)=*(p+Write_Addr);
	*(p+21)=*(p+Write_Data);
	*(p+5)=*(p+20);
	*(p+6)=*(p+10)^*(p+21);
	*(p+1)=*(p+5)+*(p+6);
	for (i=0;i<7;i++)
		*(p+1)=(*(p+1)<<1)+(*(p+1)>>15);
	*(p+9)=*(p+21)+*(p+6);
	*(p+12)=*(p+9)^*(p+6);
	*(p+14)=*(p+1)+*(p+12);
	*(p+11)=*(p+9)-*(p+12);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+14);
	ssd.OutBuff[2]=*(p+11);
	ssd.OutBuff[1]=WriMemAddr;
	ssd.OutBuff[0]=*(p+12);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+6)=ssd.InBuff[3];
	*(p+8)=ssd.InBuff[2];
	*(p+21)=ssd.InBuff[0];
	*(p+17)=*(p+21)^*(p+8);
	*(p+23)=*(p+6)+*(p+17);
	for (i=0;i<7;i++)
		*(p+23)=(*(p+23)<<1)+(*(p+23)>>15);
	*(p+15)=*(p+8)+*(p+17);
	*(p+16)=*(p+15)^*(p+17);
	*(p+19)=*(p+23)+*(p+16);
	*(p+2)=*(p+15)-*(p+16);
	v0=*(p+2);
	*(p+16)=*(p+19);
	*(p+13)=*(p+16);
	*(p+Write_Data)=v0;
	return ssd.ReturnFlag;
}

unsigned short WriteMemory3(Sense3AutoData p)
{
	unsigned short v0;
	unsigned short i;
	*(p+15)=*(p+Write_Addr);
	*(p+22)=*(p+Write_Data);
	*(p+18)=*(p+3);
	*(p+13)=*(p+15)^*(p+22);
	*(p+14)=*(p+18)+*(p+13);
	for (i=0;i<7;i++)
		*(p+14)=(*(p+14)<<1)+(*(p+14)>>15);
	*(p+3)=*(p+22)+*(p+13);
	*(p+22)=*(p+3)^*(p+13);
	*(p+15)=*(p+14)+*(p+22);
	*(p+25)=*(p+3)-*(p+22);
	ssd.FunctionCode=InAddr;
	ssd.OutWords=4;
	ssd.OutBuff[3]=*(p+15);
	ssd.OutBuff[2]=*(p+25);
	ssd.OutBuff[1]=WriMemAddr;
	ssd.OutBuff[0]=*(p+22);
	SENSE3(&ssd);
	if (ssd.ReturnFlag >= 0x80)
		return ssd.ReturnFlag;
	*(p+18)=ssd.InBuff[3];
	*(p+12)=ssd.InBuff[2];
	*(p+13)=ssd.InBuff[0];
	*(p+10)=*(p+13)^*(p+12);
	*(p+20)=*(p+18)+*(p+10);
	for (i=0;i<7;i++)
		*(p+20)=(*(p+20)<<1)+(*(p+20)>>15);
	*(p+19)=*(p+12)+*(p+10);
	*(p+22)=*(p+19)^*(p+10);
	*(p+9)=*(p+20)+*(p+22);
	*(p+2)=*(p+19)-*(p+22);
	v0=*(p+2);
	*(p+17)=*(p+9);
	*(p+7)=*(p+22);
	*(p+Write_Data)=v0;
	return ssd.ReturnFlag;
}
//加密部分结束



void initApp();
void unloadApp();
void EndCheck();
void Text2Line();
void DoubleLineOut();
void Ellipse2Pline();
void ToPline();
void All2One();
void Clear();
void ExplodeBlock();
void Widen2Pline();
void Mline2Pline();
void Trace2Pline();
void Solid2Pline();
void Spline2Outline();
void Region2Outline();
void Hatch2Outline();
void ToCut();
void KillFat();


Sense3AutoData ss3ad;


/*********************************************/
/*                                           */
/*            基本函数部分                   */
/*                                           */
/*********************************************/


//建立一个新层
void NewLayer(char *layername)
{
	AcDbLayerTable *pltable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForWrite);	

	if( NoDog3 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

	if (!(pltable->has(layername)))
	{
		AcDbLayerTableRecord *plrecord=new AcDbLayerTableRecord();
		plrecord->setName(layername);
		pltable->add(plrecord);
		plrecord->close();		
	}
	pltable->close();
}


//建立一个新层,并获得新的ID号
void NewLayer(AcDbObjectId& id,char *layername)
{
	AcDbLayerTable* pltable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pltable, AcDb::kForWrite);	

	if( NoDog2 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

	if (!(pltable->has(layername)))
	{
		AcDbLayerTableRecord *plrecord=new AcDbLayerTableRecord();
		plrecord->setName(layername);
		pltable->add(id,plrecord);
		plrecord->close();		
	}
	pltable->close();
}


//检查选择集中存不存在非闭合多义线及圆,有则放入问题层;
//检查选择集中存不存在多义线圆以外的实体,有则报警
void endchk(ads_name ss)
{
	long num=0;
	long len=0;
    acedSSLength(ss,&len);    

	if( NoDog1 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

	for(long i=0;i<len;i++)
	{
		ads_name e;
		acedSSName(ss,i,e);
		AcDbObjectId id;
		
		if( NoDog2 )
		{
			acutPrintf("\n请您购买正版软件!");
			return;
		}
		
		acdbGetObjectId(id,e);
		AcDbEntity *pent;
	    acdbOpenAcDbEntity(pent,id,AcDb::kForWrite);		
		
		//加密狗部分开始
		if( (i % 20 ) == 0 )
		{
			unsigned short h,l;

			dataH = id.asOldId() % 65536; //0-65536，随机数，可将图形中处理的坐标数的X值取绝对值付给它
			dataL = id.asOldId() / 65536;  //0-65536，随机数，可将图形中处理的坐标数的Y值取绝对值付给它
			ss3ad[EncryptBak_Highword_Data] = dataH;
			ss3ad[EncryptBak_Lowword_Data]  = dataL;
			EncryptBak(ss3ad);
			h=ss3ad[EncryptBak_Highword_Data];
			l=ss3ad[EncryptBak_Lowword_Data];
			ss3ad[Encrypt_Highword_Data] = dataH;
			ss3ad[Encrypt_Lowword_Data]  = dataL;
			Encrypt2(ss3ad); //可用Encrypt0(ss3ad)，Encrypt1(ss3ad)，Encrypt2(ss3ad)或Encrypt3(ss3ad)

			if(ss3ad[Encrypt_Highword_Data] != h || ss3ad[Encrypt_Lowword_Data] != l)
			{
				do 
				{ 
					NoDog1 = rand();
				}while( !NoDog1 );
				do
				{ 
					NoDog2 = rand();
				}while( !NoDog2 );
				do
				{
					NoDog3 = rand();
				}while( !NoDog3 );
				do
				{
					NoDog4 = rand();
				}while( !NoDog4 );
			}
		}
		//加密狗部分结束

        if (pent->isKindOf(AcDbCircle::desc()) || (pent->isKindOf(AcDbPolyline::desc()) && ((AcDbPolyline*)pent)->isClosed() && !(((AcDbPolyline*)pent)->hasWidth())) )
			;
		else
		{
			if( NoDog4 )
			{
				acutPrintf("\n请您购买正版软件!");
				return;
			}
			NewLayer("problem");
			pent->setLayer("problem");
			num++;
		}
		pent->close();	
	}
	if (num==0)
		acutPrintf("\n OK ");
	else
		acutPrintf("\n illegal entities: %d   ",num);
}

//将实体加入到数据库中
void Join2Database(AcDbEntity *pent)
{
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForWrite);
	pBlockTable->close();	
	pBlockTableRecord->appendAcDbEntity(pent);
	pBlockTableRecord->close();
}

/*
//取得实体的外框最小点和最大点
double *GetPoints(AcDbEntity *pent1)
{
	double value[4];
	AcDbExtents extents1;
	pent1->getGeomExtents(extents1);
	AcGePoint3d minPt1=extents1.minPoint(),maxPt1=extents1.maxPoint(); 
	value[0]=maxPt1.operator[](0);
	value[1]=maxPt1.operator[](1);
	value[2]=minPt1.operator[](0);
	value[3]=minPt1.operator[](1);
	return &value;
}
*/

//取得选择集图形的中心点
void GetCenterPoint(ads_name SS,ads_point CenterPt)
{
	double MinPointX=MaxData,MinPointY=MaxData;
	double MaxPointX=MinData,MaxPointY=MinData;
	long len=0;
    acedSSLength(SS,&len);
	int i=0;
	ads_name e;
	for(;i<len;i++)
	{
		acedSSName(SS,i,e);		
		AcDbObjectId id;
		acdbGetObjectId(id,e);
		AcDbEntity *pent;
		AcDbExtents extents1;
		acdbOpenAcDbEntity(pent,id,AcDb::kForWrite);
		pent->getGeomExtents(extents1);
		AcGePoint3d minPt1=extents1.minPoint(),maxPt1=extents1.maxPoint(); 
		if(minPt1.operator[](0) < MinPointX) MinPointX =minPt1.operator[](0);
		if(minPt1.operator[](1) < MinPointY) MinPointY =minPt1.operator[](1);
		if(maxPt1.operator[](0) > MaxPointX) MaxPointX =maxPt1.operator[](0);
		if(maxPt1.operator[](1) > MaxPointY) MaxPointY =maxPt1.operator[](1);
	}
	CenterPt[0]=(MaxPointX - MinPointX) / 2.0;
	CenterPt[1]=(MaxPointY - MinPointY) / 2.0;
	CenterPt[2]=0.0;
}


/*
//计算两点一线的斜率
double slope(AcGePoint2d Point1,AcGePoint2d Point2)
{
	double x1,y1,x2,y2;

	x1 = Point1.operator[](0);
	y1 = Point1.operator[](1);

	x2 = Point2.operator[](0);
	y2 = Point2.operator[](1);
	
	if( fabs(x1 - x2) <= MinData)
	{
		return MaxData;
	}
	
	return (y1 - y2) / (x1 - x2);
}

*/


/*********************************************/
/*                                           */
/*            新命令函数部分                 */
/*                                           */
/*********************************************/

//块打破函数
void ExplodeBlock()
{
	struct resbuf eb1;
	ads_name BlockSS;
	eb1.restype = 0;
	eb1.resval.rstring = "INSERT";
	eb1.rbnext = NULL;
	if(acedSSGet("X",NULL,NULL,&eb1,BlockSS) != RTNORM) 
	{
		acutPrintf("\n没有块被引用!");
		return;
	}

	long len=0;
    acedSSLength(BlockSS,&len);
	long i=0;
	ads_name e;
	for(;i<len;i++)
	{
		acedSSName(BlockSS,i,e);		
		AcDbObjectId id;
		acdbGetObjectId(id,e);
		AcDbEntity *pent;
		acdbOpenAcDbEntity(pent,id,AcDb::kForWrite);
		AcDbVoidPtrArray ExplodeBlock;
		if(pent->explode(ExplodeBlock) == Acad::eOk) acutPrintf("\n块被打破!");
//		int len1 = ExplodeBlock.length();
//		for(int j=0;j<len1;j++) Join2Database((AcDbEntity *)ExplodeBlock.at(j));
//		pent->erase();
//		ExplodeBlock::~AcArray();
	}

	acedSSFree(BlockSS);
}



//最后检查除多义线和圆外没有别的实体
void EndCheck()
{   
	char *result=new char;
    acedGetString(0,"Select Objects/One layer/<All layer>",result);	          
    struct resbuf *rb;
    ads_name ss;
	
	//加密狗部分开始
	Sense3AutoData ss3ad;
	unsigned short h,l;
	
	dataH = 13923; //0-65536，随机数，可将图形中处理的坐标数的X值取绝对值付给它
	dataL = 8170;  //0-65536，随机数，可将图形中处理的坐标数的Y值取绝对值付给它
	ss3ad[EncryptBak_Highword_Data] = dataH;
	ss3ad[EncryptBak_Lowword_Data]  = dataL;
	EncryptBak(ss3ad);
	h=ss3ad[EncryptBak_Highword_Data];
	l=ss3ad[EncryptBak_Lowword_Data];
	ss3ad[Encrypt_Highword_Data] = dataH;
	ss3ad[Encrypt_Lowword_Data]  = dataL;
	Encrypt0(ss3ad); //可用Encrypt0(ss3ad)，Encrypt1(ss3ad)，Encrypt2(ss3ad)或Encrypt3(ss3ad)

	if(ss3ad[Encrypt_Highword_Data] != h || ss3ad[Encrypt_Lowword_Data] != l)
	{
		do 
		{ 
			NoDog1 = rand();
		}while( !NoDog1 );
		do
		{ 
			NoDog2 = rand();
		}while( !NoDog2 );
		do
		{
			NoDog3 = rand();
		}while( !NoDog3 );
		do
		{
			NoDog4 = rand();
		}while( !NoDog4 );
	}
	//加密狗部分结束

   if ( strcmp(result,"O")==0 || strcmp(result,"o")==0 )
   {
	   char layername[30];	
	   acedGetString(0,"input layer name:     ",layername);
	   rb=acutBuildList(-4, "<and", 8, layername,-4, "<or", RTDXF0, "polyline", RTDXF0, "lwpolyline",-4, "or>",-4, "and>",0);	   
	   acedSSGet("x",NULL,NULL,rb,ss);	 	   	  	   
	   acutRelRb(rb);
   }
   else if ( strcmp(result,"S")==0 || strcmp(result,"s")==0 ) 
   {	   
	   acedSSGet(NULL,NULL,NULL,NULL,ss);	   	   
   }
   else
   {
	   rb=acutBuildList(0);	   
	   acedSSGet("x",NULL,NULL,rb,ss);	   
	   acutRelRb(rb);
   }   
   delete result;
   endchk(ss);
   acedSSFree(ss);	
}



/*********************************************/
/*                                           */
/*         新定义的命令初始化部分            */
/*                                           */
/*********************************************/

void initApp()
{
	acedRegCmds->addCommand("LcdCAD","endchk","最后检查",ACRX_CMD_MODAL,EndCheck);
//	acedRegCmds->addCommand("LcdCAD","t2l","文字转化",ACRX_CMD_MODAL,Text2Line);
//	acedRegCmds->addCommand("LcdCAD","dblchk","重线检测",ACRX_CMD_MODAL,DoubleLineOut);
//	acedRegCmds->addCommand("LcdCAD","e2p","椭圆转换",ACRX_CMD_MODAL,Ellipse2Pline);
//	acedRegCmds->addCommand("LcdCAD","topl","转多义线",ACRX_CMD_MODAL,ToPline);
//	acedRegCmds->addCommand("LcdCAD","a2o","一笔划",ACRX_CMD_MODAL,All2One);
//	acedRegCmds->addCommand("LcdCAD","clear","清理",ACRX_CMD_MODAL,Clear);
	acedRegCmds->addCommand("LcdCAD","exbl","块打破",ACRX_CMD_MODAL,ExplodeBlock);
//	acedRegCmds->addCommand("LcdCAD","w2p","widen2pline",ACRX_CMD_MODAL,Widen2Pline);
//	acedRegCmds->addCommand("LcdCAD","m2p","mline2pline",ACRX_CMD_MODAL,Mline2Pline);
//	acedRegCmds->addCommand("LcdCAD","t2p","trace2pline",ACRX_CMD_MODAL,Trace2Pline);
//	acedRegCmds->addCommand("LcdCAD","s2p","solid2pline",ACRX_CMD_MODAL,Solid2Pline);
//	acedRegCmds->addCommand("LcdCAD","s2o","spline2outline",ACRX_CMD_MODAL,Spline2Outline);
//	acedRegCmds->addCommand("LcdCAD","r2o","region2outline",ACRX_CMD_MODAL,Region2Outline);
//	acedRegCmds->addCommand("LcdCAD","h2o","hatch2outline",ACRX_CMD_MODAL,Hatch2Outline);
//	acedRegCmds->addCommand("LcdCAD","tocut","tocut",ACRX_CMD_MODAL,ToCut);
//	acedRegCmds->addCommand("LcdCAD","kf","killfat",ACRX_CMD_MODAL,KillFat);
}


void unloadApp()
{
	acedRegCmds->removeGroup("LcdCAD");
}


extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg,void *ptr)
{
        switch(msg)
        {
                case AcRx::kInitAppMsg:
                        acrxDynamicLinker->unlockApplication(ptr);
                        acrxRegisterAppMDIAware(ptr);

						//加密狗部分
						acutPrintf("\n清溢精密光电(深圳)有限公司");
						acutPrintf("\nＣＡＭ工程部23501设计室");
						acutPrintf("\nTel:0755-6638794-19 E-Mail:LcdCAD@szonline.net\n");
						NoDog1 = 0;
						NoDog2 = 0;
						NoDog3 = 0;
						NoDog4 = 0;
						if(OpenSense3())
						{
							do 
							{ 
								NoDog1 = rand();
							}while( !NoDog1 );
							break;
							do 
							{ 
								NoDog2 = rand();
							}while( !NoDog2 );
							break;
							do
							{ 
								NoDog3 = rand();
							}while( !NoDog3 );
							break;
							do 
							{ 
								NoDog4 = rand();
							}while( !NoDog4 );
							break;
						}
						else
						{
							NoDog1 = 0;
							NoDog2 = 0;
							NoDog3 = 0;
							NoDog4 = 0;
						}
						//加密狗部分结束

						initApp();
						break;
                case AcRx::kUnloadAppMsg:
                        unloadApp();
						acutPrintf("\n多谢使用!您的反馈是对我们最大的支持!");
						acutPrintf("\n反馈地址 E-Mail:LcdCAD@szonline.net\n");
						break;
				default:
						break;
        }
        return AcRx::kRetOK;
}


//主程序功能模块
/*
void testDog()
{
	Sense3AutoData ss3ad;
	unsigned short i,j;

	//置于程序最前面
	if(OpenSense3())
	{
		//可以改用对话框方式
		do 
		{ 
			NoDog1 = rand();
		}while( !NoDog1 );
		acutPrintf("\n清溢精密光电(深圳)有限公司");
		do 
		{ 
			NoDog2 = rand();
		}while( !NoDog2 );
		acutPrintf("\nＣＡＭ工程部23501设计室");
		do
		{ 
			NoDog3 = rand();
		}while( !NoDog3 );
		acutPrintf("\nTel:0755-6638794-19 E-Mail:LcdCAD@szonline.net");
		do 
		{ 
			NoDog4 = rand();
		}while( !NoDog4 );
	}
	else
	{
		//可以改用对话框方式
		NoDog1 = 0;
		acutPrintf("\n清溢精密光电(深圳)有限公司");
		NoDog2 = 0;
		acutPrintf("\nＣＡＭ工程部23501设计室");
		NoDog3 = 0;
		acutPrintf("\nTel:0755-6638794-19 E-Mail:LcdCAD@szonline.net");
		NoDog4 = 0;
	}

	//下面四段随机放在程序里面，可以多放几次，位置任一
	//测试软硬变换的一致性，从而验证狗的存在否
	
	dataH = 13923; //0-65536，随机数，可将图形中处理的坐标数的X值取绝对值付给它
	dataL = 8170;  //0-65536，随机数，可将图形中处理的坐标数的Y值取绝对值付给它
	ss3ad[EncryptBak_Highword_Data] = dataH;
	ss3ad[EncryptBak_Lowword_Data]  = dataL;
	EncryptBak(ss3ad);
	i=ss3ad[EncryptBak_Highword_Data];
	j=ss3ad[EncryptBak_Lowword_Data];
	ss3ad[Encrypt_Highword_Data] = dataH;
	ss3ad[Encrypt_Lowword_Data]  = dataL;
	Encrypt0(ss3ad); //可用Encrypt0(ss3ad)，Encrypt1(ss3ad)，Encrypt2(ss3ad)或Encrypt3(ss3ad)

	if(ss3ad[Encrypt_Highword_Data] != i || ss3ad[Encrypt_Lowword_Data] != j)
	{
		do 
		{ 
			NoDog1 = rand();
		}while( !NoDog1 );
		do
		{ 
			NoDog2 = rand();
		}while( !NoDog2 );
		do
		{
			NoDog3 = rand();
		}while( !NoDog3 );
		do
		{
			NoDog4 = rand();
		}while( !NoDog4 );
	}

	//测试软变换与硬反变换的一致性，从而验证狗的存在否
	
	dataH = 13923; //0-65536，随机数，可将图形中处理的坐标数的X值取绝对值付给它
	dataL = 8170;  //0-65536，随机数，可将图形中处理的坐标数的Y值取绝对值付给它
	ss3ad[EncryptBak_Highword_Data] = dataH;
	ss3ad[EncryptBak_Lowword_Data]  = dataL;
	EncryptBak(ss3ad);
	i=ss3ad[EncryptBak_Highword_Data];
	j=ss3ad[EncryptBak_Lowword_Data];
	ss3ad[Decrypt_Highword_Data] = i;
	ss3ad[Decrypt_Lowword_Data]  = j;
	Decrypt0(ss3ad); //可用Decrypt0(ss3ad),Decrypt1(ss3ad),Decrypt2(ss3ad)或Decrypt3(ss3ad)

	if(ss3ad[Decrypt_Highword_Data] != dataH || ss3ad[Decrypt_Lowword_Data] != dataL)
	{
		do
		{
			NoDog1 = rand();
		}while( !NoDog1 );
		do
		{ 
			NoDog2 = rand();
		}while( !NoDog2 );
		do
		{
			NoDog3 = rand();
		}while( !NoDog3 );
		do
		{
			NoDog4 = rand();
		}while( !NoDog4 );
	}

	//测试硬反变换与软反变换的一致性，从而验证狗的存在否
	
	dataH = 13923; //0-65536，随机数，可将图形中处理的坐标数的X值取绝对值付给它
	dataL = 8170;  //0-65536，随机数，可将图形中处理的坐标数的Y值取绝对值付给它
	ss3ad[DecryptBak_Highword_Data] = dataH;
	ss3ad[DecryptBak_Lowword_Data]  = dataL;
	DecryptBak(ss3ad);
	i=ss3ad[DecryptBak_Highword_Data];
	j=ss3ad[DecryptBak_Lowword_Data];
	ss3ad[Decrypt_Highword_Data] = dataH;
	ss3ad[Decrypt_Lowword_Data]  = dataL;
	Decrypt0(ss3ad); //可用Decrypt0(ss3ad),Decrypt1(ss3ad),Decrypt2(ss3ad)或Decrypt3(ss3ad)

	if(ss3ad[Decrypt_Highword_Data] != i || ss3ad[Decrypt_Lowword_Data] != j)
	{
		do
		{
			NoDog1 = rand();
		}while( !NoDog1 );
		do
		{ 
			NoDog2 = rand();
		}while( !NoDog2 );
		do
		{
			NoDog3 = rand();
		}while( !NoDog3 );
		do
		{
			NoDog4 = rand();
		}while( !NoDog4 );
	}

	//测试软反变换与硬变换的一致性，从而验证狗的存在否
	
	dataH = 13923; //0-65536，随机数，可将图形中处理的坐标数的X值取绝对值付给它
	dataL = 8170;  //0-65536，随机数，可将图形中处理的坐标数的Y值取绝对值付给它
	ss3ad[DecryptBak_Highword_Data] = dataH;
	ss3ad[DecryptBak_Lowword_Data]  = dataL;
	DecryptBak(ss3ad);
	i=ss3ad[DecryptBak_Highword_Data];
	j=ss3ad[DecryptBak_Lowword_Data];
	ss3ad[Encrypt_Highword_Data] = i;
	ss3ad[Encrypt_Lowword_Data]  = j;
	Encrypt0(ss3ad); //可用Encrypt0(ss3ad)，Encrypt1(ss3ad)，Encrypt2(ss3ad)或Encrypt3(ss3ad)

	if(ss3ad[Encrypt_Highword_Data] != dataH || ss3ad[Encrypt_Lowword_Data] != dataL)
	{
		do
		{
			NoDog1 = rand();
		}while( !NoDog1 );
		do
		{ 
			NoDog2 = rand();
		}while( !NoDog2 );
		do
		{
			NoDog3 = rand();
		}while( !NoDog3 );
		do
		{
			NoDog4 = rand();
		}while( !NoDog4 );
	}


	//请将下列四段代码分别加到程序的任一处，至少要加足十二段	
	//不要放在一起，可不分前后，多多益善。

	//第一段
	if( NoDog1 )
	{
		CaseFlag = NoDog1 % 12;
		switch( CaseFlag )
		{
			case 0:		goto MessageLineOne;
			case 1:		goto MessageLineTwo;
			case 2:		goto MessageLineThree;
			case 3:		goto MessageLineFour;
			case 4:		goto MessageLineFive;
			case 5:		goto MessageLineSix;
			case 6:		goto MessageLineSeven;
			case 7:		goto MessageLineEight;
			case 8:		goto MessageLineNine;
			case 9:		goto MessageLineTen;
			case 10:	goto MessageLineEleven;
			case 11:	goto MessageLineTwelve;
		}
	}
	
	//第二段
	if( NoDog2 )
	{
		CaseFlag = NoDog2 % 12;
		switch (CaseFlag)
		{
			case 0:		goto MessageLineOne;
			case 1:		goto MessageLineTwo;
			case 2:		goto MessageLineThree;
			case 3:		goto MessageLineFour;
			case 4:		goto MessageLineFive;
			case 5:		goto MessageLineSix;
			case 6:		goto MessageLineSeven;
			case 7:		goto MessageLineEight;
			case 8:		goto MessageLineNine;
			case 9:		goto MessageLineTen;
			case 10:	goto MessageLineEleven;
			case 11:	goto MessageLineTwelve;
		}
	}

	//第三段
	if( NoDog3 )
	{
		CaseFlag = NoDog3 % 12;
		switch (CaseFlag)
		{
			case 0:		goto MessageLineOne;
			case 1:		goto MessageLineTwo;
			case 2:		goto MessageLineThree;
			case 3:		goto MessageLineFour;
			case 4:		goto MessageLineFive;
			case 5:		goto MessageLineSix;
			case 6:		goto MessageLineSeven;
			case 7:		goto MessageLineEight;
			case 8:		goto MessageLineNine;
			case 9:		goto MessageLineTen;
			case 10:	goto MessageLineEleven;
			case 11:	goto MessageLineTwelve;
		}
	}

	//第四段
	if( NoDog4 )
	{
		CaseFlag = NoDog4 % 12;
		switch (CaseFlag)
		{
			case 0:		goto MessageLineOne;
			case 1:		goto MessageLineTwo;
			case 2:		goto MessageLineThree;
			case 3:		goto MessageLineFour;
			case 4:		goto MessageLineFive;
			case 5:		goto MessageLineSix;
			case 6:		goto MessageLineSeven;
			case 7:		goto MessageLineEight;
			case 8:		goto MessageLineNine;
			case 9:		goto MessageLineTen;
			case 10:	goto MessageLineEleven;
			case 11:	goto MessageLineTwelve;
		}
	}

	//请将下面十二段代码分别放去程序的任一位置，要全部加入，
	//不要放在一起，可不分前后。

MessageLineOne:
	if( NoDog1 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

MessageLineTwo:
	if( NoDog3 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

MessageLineThree:
	if( NoDog4 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

MessageLineFour:
	if( NoDog2 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

MessageLineFive:
	if( NoDog4 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

MessageLineSix:
	if( NoDog3 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

MessageLineSeven:
	if( NoDog2 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

MessageLineEight:
	if( NoDog1 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

MessageLineNine:
	if( NoDog1 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

MessageLineTen:
	if( NoDog2 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

MessageLineEleven:
	if( NoDog3 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

MessageLineTwelve:
	if( NoDog4 )
	{
		acutPrintf("\n请您购买正版软件!");
		return;
	}

	//置于程序结束前
	CloseSense3();
	acutPrintf("\n谢谢您使用我们的软件，您的反馈是对我们最大的支持。");
}

*/
/*	acedRegCmds->addCommand("LcdCAD", "endchk","endchk",ACRX_CMD_MODAL,endchk);
	acedRegCmds->addCommand("LcdCAD","widen","widen",ACRX_CMD_MODAL,widen);
	acedRegCmds->addCommand("LcdCAD","wp2p","wp2p",ACRX_CMD_MODAL,wp2p);
	acedRegCmds->addCommand("LcdCAD","fit","fit",ACRX_CMD_MODAL,fit);
	acedRegCmds->addCommand("LcdCAD","t2pl","t2pl",ACRX_CMD_MODAL,t2pl);
	acedRegCmds->addCommand("LcdCAD","join_pl","join_pl",ACRX_CMD_MODAL,join_pl);
	acedRegCmds->addCommand("LcdCAD","pl","pl",ACRX_CMD_MODAL,pl);
	acedRegCmds->addCommand("LcdCAD","newpl","newpl",ACRX_CMD_MODAL,newpl);
	acedRegCmds->addCommand("LcdCAD","restore","restore",ACRX_CMD_MODAL,restore);
	acedRegCmds->addCommand("LcdCAD","plchk","plchk",ACRX_CMD_MODAL,plchk);
	acedRegCmds->addCommand("LcdCAD","drawpl","drawpl",ACRX_CMD_MODAL,drawpl);
	acedRegCmds->addCommand("LcdCAD", "v_chk","v_chk",ACRX_CMD_MODAL,v_chk);	
	acedRegCmds->addCommand("LcdCAD", "to_pl","to_pl",ACRX_CMD_MODAL,to_pl);		
	acedRegCmds->addCommand("LcdCAD","ilayer","ilayer",ACRX_CMD_MODAL,ilayer);	
	acedRegCmds->addCommand("LcdCAD","self","self",ACRX_CMD_MODAL,self);	
	acedRegCmds->addCommand("LcdCAD","cut","cut",ACRX_CMD_MODAL,cut);		
	acedRegCmds->addCommand("LcdCAD","createhatch","createhatch",ACRX_CMD_MODAL,createhatch);	
	acedRegCmds->addCommand("LcdCAD","test","test",ACRX_CMD_MODAL,test);
	acedRegCmds->addCommand("LcdCAD","test1","test1",ACRX_CMD_MODAL,test1);
	acedRegCmds->addCommand("LcdCAD","test2","test2",ACRX_CMD_MODAL,test2);
	acedRegCmds->addCommand("LcdCAD","tp","tp",ACRX_CMD_MODAL,tp);
	acedRegCmds->addCommand("LcdCAD","arc_chk","arc_chk",ACRX_CMD_MODAL,arc_chk);
	acedRegCmds->addCommand("LcdCAD","to_0","to_0",ACRX_CMD_MODAL,to_0);
	acedRegCmds->addCommand("LcdCAD","frm","frm",ACRX_CMD_MODAL,frm);
	acedRegCmds->addCommand("LcdCAD","dev","dev",ACRX_CMD_MODAL,dev);
	//acedRegCmds->addCommand("LcdCAD","merge","merge",ACRX_CMD_MODAL,merge);
	acedRegCmds->addCommand("LcdCAD","pu","pu",ACRX_CMD_MODAL,pu);
	acedRegCmds->addCommand("LcdCAD","ers","ers",ACRX_CMD_MODAL,eraseEnt);
	acedRegCmds->addCommand("LcdCAD","hold","hold",ACRX_CMD_MODAL,holdEnt);
	acedRegCmds->addCommand("LcdCAD","num","num",ACRX_CMD_MODAL,num);
	acedRegCmds->addCommand("LcdCAD","hd","hd",ACRX_CMD_MODAL,hd);
	acedRegCmds->addCommand("LcdCAD","mergeo","mergeo",ACRX_CMD_MODAL,mergeOneLayer);
	acedRegCmds->addCommand("LcdCAD","lytype","lytype",ACRX_CMD_MODAL,lytype);
	acedRegCmds->addCommand("LcdCAD","vis","vis",ACRX_CMD_MODAL,vis);
	acedRegCmds->addCommand("LcdCAD","layerinf","layerinf",ACRX_CMD_MODAL,layerInf);
	acedRegCmds->addCommand("LcdCAD","txtpl2pl","txtpl2pl",ACRX_CMD_MODAL,txtpl2pl);
	acedRegCmds->addCommand("LcdCAD","circuit","circuit",ACRX_CMD_MODAL,circuit);
	acedRegCmds->addCommand("LcdCAD","restore","restore",ACRX_CMD_MODAL,restore);
	acedRegCmds->addCommand("LcdCAD","minDist","minDist",ACRX_CMD_MODAL,minDist);
	acedRegCmds->addCommand("LcdCAD","minWidth","minWidth",ACRX_CMD_MODAL,minWidth);
	acedRegCmds->addCommand("LcdCAD","shortcheck","shortcheck",ACRX_CMD_MODAL,shortCheck);
	acedRegCmds->addCommand("LcdCAD","opencheck","opencheck",ACRX_CMD_MODAL,openCheck);
	acedRegCmds->addCommand("LcdCAD","omiga","omiga",ACRX_CMD_MODAL,omiga);
	acedRegCmds->addCommand("LcdCAD","widthCheck","widthCheck",ACRX_CMD_MODAL,widthCheck);
	acedRegCmds->addCommand("LcdCAD","distCheck","distCheck",ACRX_CMD_MODAL,distCheck);
	acedRegCmds->addCommand("LcdCAD","Check","Check",ACRX_CMD_MODAL,check);
	acedRegCmds->addCommand("LcdCAD","minWidth","minWidth",ACRX_CMD_MODAL,minWidth);
	acedRegCmds->addCommand("LcdCAD","sln","sln",ACRX_CMD_MODAL,getStartLn);
	acedRegCmds->addCommand("LcdCAD","hdpl","hdpl",ACRX_CMD_MODAL,handlePl);
	acedRegCmds->addCommand("LcdCAD","test","test",ACRX_CMD_MODAL,test);
	acedRegCmds->addCommand("LcdCAD","setup","setup",ACRX_CMD_MODAL,setup);
	acedRegCmds->addCommand("LcdCAD","create","create",ACRX_CMD_MODAL,create);
	acedRegCmds->addCommand("LcdCAD","destroy","destroy",ACRX_CMD_MODAL,destroy);
	acedRegCmds->addCommand("LcdCAD","cvt","cvt",ACRX_CMD_MODAL,convert);
*/