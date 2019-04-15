/*   (C) Copyright 1998 SenseLock Technologies Inc. */

//////////////////////////////////////////////////////////
// D:\senseIII\vc\AutoC011.c : the file implemente the key functions
//

#include "windows.h"
#include "D:\senseIII\vc\AutoC011.h"
#include "sense3.h"

#define	InAddr	33
#define	ReadMemAddr	1
#define	DecAddr	3
#define	EncAddr	16
#define	WriMemAddr	29
SENSE3DATA ssd;

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
	unsigned short v0,v1;
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
	unsigned short v0,v1;
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
	unsigned short v0,v1;
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
	unsigned short v0,v1;
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
	unsigned short v0,v1;
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
	unsigned short v0,v1;
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
	unsigned short v0,v1;
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
