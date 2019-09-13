/*************************************************************************
Copyright (C), 2017-12-1, shiyuan precision maskmaking. Co., Ltd.
FileName: pointMap.h
Author: 
Version:
Date: //zhg 1.0
Description: // 1.对选中的实体集合建立顶点表，供查询;
             // 2.使用map结构; 以点坐标的x、y为查询key值; x的map嵌套y的map；
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改
		// 者及修改内容简述
eg： Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20190822_segments_intersections
#define  ZHOUHUAGANG_20190822_segments_intersections



#include <vector>
#include <map>   
#include <algorithm>
//#include <>


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

#include "..\StdAfx.h"
#include "TCHAR.h"

#include <vector>
#include <list>
#include <map>


//建立3个结构：
//1.存储所有线段的端点list（包括新发现的交点;
//2.存储扫描线上的点set（二叉树？）；
//3.存储发现的交点;

//几个问题点：
//1.端点重合的线段怎么表达;
//2.圆弧怎么表示成线段？: 弧段用通过中心的垂线和水平线划分成四段; 每段弧是单调的;
//3.交点是下端点；

//本软件功能：求N个线段的交点集;
//采用扫描线相交算法;
//


//指示弧段方位：
//弧段用通过中心的垂线和水平线划分成四段:左上，左下，右上，右下;
enum EPartOfArc
{
	LEFT_TOP,
	LEFT_BOTTOM,
	RIGHT_TOP,
	RIGHT_BOTTOM,
	JUST_ALONE，//表示这个弧本身就是单调的;
	NONE_NONE
};



//枚举，指示线段类型
enum ETypeOfArc
{
	Line_Type,
	Arc_Type,
	Circle_Type,
	Polyline_Type,
	AcGeCircArc2d_Type，
	None_Type //problem type;
};

/*
类名：CSegement
用途：表示一个线段，可能是一条直线，也可能是一条弧;   
一条多一线分解成多个直线段和弧段
弧段划分成四段：弧段用通过中心的垂线和水平线划分成四段; 每段弧是单调的
*/
class CSegement
{
public:
	CSegement();
	CSegement(IN AcDbLine* dbLinePtr);
	CSegement(IN AcDbPolyline* polylinePtr,IN int indexVertex,IN int arcPartNumber);
	CSegement(IN AcDbCircle* circlePtr,IN int arcPartNumber);
	CSegement(IN AcDbArc* arcPtr, IN int arcPartNumber);
	
public:
	//线段的x坐标
	double m_x;

	//本线段，是弧？是线？是多义线？
	ETypeOfArc m_myselfType ; 
	AcDbArc* m_arcPtr;
	AcDbLine* m_LinePtr;	
	EPartOfArc m_whichPart;

	//母线段，是弧？是圆？是多义线？:不可能是线line;
	ETypeOfArc m_parentType ;
	AcDbArc* m_arcPtr;
	AcDbCircle* m_circlePtr;
	//AcDbLine* m_LinePtr;
	AcGeCircArc2d* m_circArc2d;
	AcDbPolyline* m_polylinePtr;

	//祖母线段，是弧？是线？是多义线？
	ETypeOfArc m_grandmaType;
	//AcDbArc* m_arcPtr;
	//AcDbLine* m_LinePtr;
	AcDbPolyline* m_polylinePtr; 
public:
	;
};


#endif  //ZHOUHUAGANG_20190822_segments_intersections