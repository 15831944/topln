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

#ifndef  OWIN_FINDTHEGAPBTWPOINTS_H_
#define  OWIN_FINDTHEGAPBTWPOINTS_H_


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
#include "selectss.h"

using namespace std;

void findPtPairDistLessThan();

//=================
//class CFindGapBtwPoints
//查找点对的要求：
//1.不重叠的点; (CPointMap类已经避免重叠点)
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
	void insertPoints(IN const AcGePoint3d pt3d,IN const AcDbObjectId objId); 
	bool extrPntsFromEntity(const ads_name ssUnit,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte,OUT AcDbObjectId& objId); 
	void parsePointPairs();  //计算不超过一定距离的点对;
	void filterPointPairs();  //过滤点对;不符合的点对删除掉;  

public:
	bool inputAdsName(IN ads_name ss); 
	bool inputMinDistByUser(const double minDist);
	//void getPickSet(ads_name ss);    //提示用户选取实体集;   
	void getPointPair(OUT vector<pair<void*,void*>>&  vPointPairs);  	
	void addOneLine(const AcGePoint3d pt1,const AcGePoint3d pt2);  //connect the gap;    
	void addAllLine();  //全部连接起来; 
	void zoomArea(const AcGePoint3d pt1,const AcGePoint3d pt2);  //zoom by two points;   

private:
	CPointMap m_pointsMap;  //存储点坐标; 
	ads_name m_ssall;  //用户选择的、过滤后的实体集合;    
	AcDbEntity* m_pEnt; 
	vector<pair<void*,void*>> m_vPointPairs;    
	vector<SAttachData*> m_vAttDataPtrs;  //gathered to release the memory;    
	double m_dist;  //用户输入的最距离，小于此距离的点对被计算出来; 
};


#endif //OWIN_FINDTHEGAPBTWPOINTS_H_   