//*************************************************************************
//Copyright (C), 2018-08-14, ���ֽ���  Mountain & Sesame Tech. Co., Ltd.
//File name:  findTheGapBtwPoints.h
//Author: 
//Version:
//Date: 2018-08-14
//Description: 
//��cpp�ļ�˵��;
//Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵�� 
//1. ....
//History: 
//�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ���
//1. Date:
//	Author:
//	Modification: 
//2. ... 
//*************************************************************************

#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_OPEN_ALL_LAYER_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_OPEN_ALL_LAYER_H


//#include "afxwin.h"
//#include "stdafx.h"
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

#include <string>
#include <iostream>

#include "pointMap.h"

using namespace std;


//=================

//=================

class CFindGapBtwPoints
{
public:
	CFindGapBtwPoints();
	~CFindGapBtwPoints();

private:	
	void insertLinePoints(const AcDbLine* linePtr);
	void insertPlinePoints(const AcDbPolyline* plPtr);
	void insertArcPoints(const AcDbArc* arcptr);
	void insertPoints(const AcGePoint3d* pt3d);
	void extrPntsFromEntity(const ads_name ssUnit);   

public:
	void getPickSet(ads_name ss); 
	void getPointPair(vector<pair<void*,void*>>& m_vPointPairs);    
	void addLine(const AcGePoint3d pt1,const AcGePoint3d pt2);  //connect the gap;  

private:
	CPointMap m_pointsMap;  //�洢������;
	ads_name ss_all;  //�û�ѡ��ġ����˺��ʵ�弯��; 
	AcDbEntity* m_pEnt; 
	vector<pair<void*,void*>> m_vPointPairs;
}