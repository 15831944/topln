//*************************************************************************
//Copyright (C), 2018-08-14, 须弥芥子  Mountain & Sesame Tech. Co., Ltd.
//File name:  findTheGapBtwPoints.h
//Author: 
//Version:
//Date: 2018-08-14
//Description: 
//见cpp文件说明;
//Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明 
//1. ....
//History: 
//修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
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
	CPointMap m_pointsMap;  //存储点坐标;
	ads_name ss_all;  //用户选择的、过滤后的实体集合; 
	AcDbEntity* m_pEnt; 
	vector<pair<void*,void*>> m_vPointPairs;
}