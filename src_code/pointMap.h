/*************************************************************************
Copyright (C), 2017-12-1, shiyuan procision maskmaking. Co., Ltd.
FileName: pointMap.h
Author: 
Version:
Date: //zhg 1.0
Description: // 1.��ѡ�е�ʵ�弯�Ͻ������������ѯ;
             // 2.ʹ��map�ṹ; �Ե������x��yΪ��ѯkeyֵ; x��mapǶ��y��map��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
eg�� Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20171201_POINTMAP_DOT_H
#define  ZHOUHUAGANG_20171201_POINTMAP_DOT_H



#include <vector>
#include <map>   
#include <algorithm>


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

#include  "opt.h"
#include "elapsetime.h"
#include "basefunc.h"

using namespace std;

void testPointMapClass();

//map�ȽϺ���
//֪��Ϊɶ��ô����ȽϺ����𣿣���Ϊ��map�ڲ�����ôʹ��dblcmp�ģ�
//1�������Ƿ���ȣ��Ȳ��� i�Ƿ�С��j����������Ȼ�����j�Ƿ�С��i���������� �������ж���ȣ�
//2������i�Ƿ�С��j��ֱ�ӾͲ��Գ�����;
//3������i�Ƿ����j�����1����2�������������ʹ�����;
struct dblcmp
{
	bool operator()(const double i,const double j) const
	{
		return (i < j && abs(i - j) > 0.000001);
	}
};


//keyΪy����
struct SYData
{
	SYData();
	SYData(IN const SYData& src);  //copy constructor;
	~SYData();
	double m_y;
	AcGePoint3d pt;
	int m_PointIndex;  //�����ţ���ͼ�ṹ�У�   
};


//keyΪx����
//ptIndex:��������ͼ�е����;
struct SXData
{
	SXData();
	~SXData();
	SXData(IN const SXData& src);  //copy constructor;
	double m_x;
	int m_nDotNum; //С����������λ;
	map<double,SYData,dblcmp> m_pPointMap;  //Ƕ��map;

	bool find(IN const double yVal,OUT int& ptIndex);  //����y-key; 
	map<double,SYData,dblcmp>::iterator insert(IN const double yVal,IN const int ptIndex); //����y key; 
	unsigned int erase(IN const double yVal);  //delete;
	void print();  //���ԣ���ӡ��Ϣ;
};


//��x��y������Ϊkey������Ƕ��map���ṩ���롢��ѯ��ɾ���ȹ���;
//ϣ������ߵ������ѯ�ٶ�;
class CPointMap
{
public:
	CPointMap();
	~CPointMap();

private:
	map<double,SXData,dblcmp> m_mapXcoord;
	int m_nDotNum;   //С����λ��;
	bool isDotNumSet; //�ж�С��λ���Ƿ��Ѿ����ã����ù��˾Ͳ�����������;
	

public:
	void insert(IN const double x,IN const double y,IN const int ptIndex);  //����
	void insert(IN const AcGePoint3d pt,IN const int ptIndex);
	bool find(IN const double x,IN const double y,OUT int& ptIndex);   //����;
	bool find(IN const AcGePoint3d pt,OUT int& ptIndex);
	bool erase(IN const double x,IN const double y);  //ɾ��;
	bool erase(IN const AcGePoint3d pt);
	bool setDotNum(IN const int nDotNum);
	void print();
	bool printMap();  //��ӡ��map�ĵ�;

public:
	static double transByDotNum(IN const double xyVal,IN const int nDotNum); //��xyֵ���й��ˣ�����ָ��λ����С��;
	static bool isEqual(IN const double d1,IN const double d2,IN const int m_nDotNum); //����Ƿ���ȣ�����Χ��);
};



#endif  //ZHOUHUAGANG_20171201_POINTMAP_DOT_H