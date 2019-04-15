/* (C) Copyright 1998 SenseLock Technologies Inc. */

// FunctionCode:
#define Sense3Init	0xffff
#define Sense3Exit	0x400	//1024

// Success ReturnFlag:
#define APISuccess		0 //success
#define APIWriteCheckFail	8 //write-read check fail
#define APIDoubleWrite		4 //write twice,the second be ignored
#define APIDataAddrOverflow	2 //read or write out of data area
#define APICodeAddrOverflow	1 //program run out of code area

// Fail ReturnFlag:
#define APINoSense		0x80 //no sense connected
#define APIChkpassTrap		0x81 //password check timeout
#define APIChkpassFail		0x82 //password check fail
#define APIExeTrap		0x83 //execute timeout
#define APIExeFail		0x84 //execute fail
#define APIEEwriteTrap		0x85 //write delay timeout
#define RetWriteFail		0x86 //write fail
#define RetWriteDummy		0x87 //write invalid
#define APIInitFirst		0x8a //init required
#define APINoDriver		0x8b //no sense3 driver

typedef struct {
 unsigned short ReturnFlag;	//function result
 unsigned short FunctionCode;	//function address
 unsigned short Password[3];	//password
 unsigned short OutWords;	//number of words send to sense3
 unsigned short OutBuff[4];	//out buffer
 unsigned short InWords;	//number of words received from sense3
 unsigned short InBuff[4];	//in buffer
 unsigned short Setting;	//ignored
 unsigned short reserved[32];	//reserved for system, can't change !
} SENSE3DATA;


#ifdef __cplusplus
extern "C" {
#endif

void WINAPI PUTDATA(int data,char *str,int pos);
int WINAPI GETDATA(char *str,int pos);
void WINAPI SENSE3(SENSE3DATA *p);
void WINAPI PSENSE3(SENSE3DATA *p);

#ifdef __cplusplus
}
#endif
