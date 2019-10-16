/*************************************************************************
Copyright (C), 2016-6-12, XuMiJieZi procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: 
Version:
Date: // hugo��1.0
Description: // ��ͼ����arc��line��polyline��ȡ
Others: // �������ݵ�˵��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
eg�� Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20160612_REPORT_ENTITIES_NUMBER_DOT_H
#define  ZHOUHUAGANG_20160612_REPORT_ENTITIES_NUMBER_DOT_H


#include "..\StdAfx.h"

#include <vector>
#include <stack>    //��ջ��װ��·��(��)��
#include <string>
#include <iostream>
#include <sstream>


#include <stdlib.h>
#include <ctype.h>
#include <aced.h>
#include <rxregsvc.h>
#include <adscodes.h>
#include <dbsymtb.h>
#include <dblayout.h>
#include <dbapserv.h>
#include <acdblmgr.h>
#include <dbents.h>
#include <dbpl.h>
#include <dbid.h>
#include <acutads.h>
#include <string.h>
#include <stdio.h>
#include <acedads.h>
#include <dbmain.h>
#include <geplin2d.h>
#include <math.h>
#include <acarray.h>
#include <dbdict.h>
#include <dbgroup.h>
#include <rxobject.h>
#include <gelnsg2d.h>
#include <gevec2d.h>

#include "elapsetime.h"
#include "selectss.h"

void  reportAllEnt();

//#pragma comment("rxapi.lib")

using namespace std;



//ÿ��ʵ���Ӧһ��SData��
//���ÿ��ʵ������Ҳͳ��������ṹ�
struct SData
{
	wstring m_className;
	int m_number;	         //������ʾ�պ϶�����������
	bool isPolylineClosed;	//��һ�����ܣ�ʶ��������Ƿ�պϣ�
	int m_numOfNotClosed;  //δ�պϵĶ�����������

	SData();
};


class CDataArray 
{
public:
	CDataArray();
	~CDataArray();

private:
	vector<SData> m_nameNumber;   
	wstring m_info;   //���ձ�����Ϣ;

public:
	bool findAndUpdate(SData* psd); //�����ھͼ��룬���ھ����Ӽ���;
	
private:
	void push_back(SData sdat);

public:	
	void printData();
	wstring printInfo();

};



//����ʵ������ͣ�������
class CEntitiesReport
{
public:
	CEntitiesReport();
	~CEntitiesReport();

private:
	ads_name ssAll;
	long m_sslength;
	SData m_sdt;  
	CDataArray m_typeAndNumber; //ʵ����������������ڴˣ�

public:	
	void analysisEntity(ads_name ss); //����ͳ��ʵ�壻
};





#endif  //ZHOUHUAGANG_20160612_REPORT_ENTITIES_NUMBER_DOT_H