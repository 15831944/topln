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

#ifndef  OWAL_FINDTHEGAPSBTWPOINTS_H
#define  OWAL_FINDTHEGAPSBTWPOINTS_H


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

#include "src_code\pointMap.h"

using namespace std;


//=================
//class CFindGapBtwPoints
//查找点对的要求：
//1.不重叠的点;
//2.点对距离不大于指定距离list;
//3.不在同一线段上;
//点对之间不能有路径;
//=================

class CFindGapBtwPoints
{
public:
	CFindGapBtwPoints();
	~CFindGapBtwPoints();  

private:
	bool getLineEndPoints(IN const AcDbLine* linePtr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte); 
	bool getPolylineEndPoints(const AcDbPolyline* plPtr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte);
	bool getArcEndPoints(const AcDbArc* arcptr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte);
	bool isTwoPointsOverlapped(IN AcGePoint3d& pts,IN AcGePoint3d& pte);  //判断两个点是否重合(相等);  
	bool insertPoints(const AcGePoint3d* pt3d); 
	void extrPntsFromEntity(const ads_name ssUnit);   

public:
	void getPickSet(ads_name ss); 
	void getPointPair(vector<pair<void*,void*>>& m_vPointPairs);  
	void addOneLine(const AcGePoint3d pt1,const AcGePoint3d pt2);  //connect the gap;  
	void addAllLine();  //全部连接起来;
	void zoomArea(const AcGePoint3d pt1,const AcGePoint3d pt2);  //zoom by two points;   

private:
	CPointMap m_pointsMap;  //存储点坐标;   
	ads_name ss_all;  //用户选择的、过滤后的实体集合;       
	AcDbEntity* m_pEnt; 
	vector<pair<AcGePoint3d,AcGePoint3d>> m_vPointPairs;     
}


#endif  //OWAL_FINDTHEGAPSBTWPOINTS_H