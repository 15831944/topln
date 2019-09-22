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

#include <map>   
#include <algorithm>
#include <vector>
#include <list>
//--------------------
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

#include "opt.h"




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
	JUST_ALONE， //表示这个弧本身就是单调的;
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
	~CSegement();

public:
	//线段的端点x坐标	
	double m_xTop;
	double m_yTop;
	double m_xBottom;
	double m_yBottom;

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

	//祖母线段，是弧？是线？是多义线?  
	ETypeOfArc m_grandmaType;
	//AcDbArc* m_arcPtr;
	//AcDbLine* m_LinePtr;
	AcDbPolyline* m_polylinePtr; 

public:
	bool initCSegement(IN AcDbLine* dbLinePtr);
	bool initCSegement(IN AcDbPolyline* polylinePtr,IN int indexVertex,IN int arcPartNumber); 
	bool initCSegement(IN AcDbCircle* circlePtr,IN int arcPartNumber); 
	bool initCSegement(IN AcDbArc* arcPtr, IN int arcPartNumber);
};




/*
弧段提取线段CSegement;
将弧段分成四部分：以圆心画垂直线和水平线，将圆弧切割成四部分；
这样每部分都是单调的弧;
*/
class CArcToSegment
{
public:
	CArcToSegment(); 
	~CArcToSegment(); 
	CArcToSegment(AcGeCircArc2d* pGeArc2d);

private:
	AcGeCircArc2d* m_geArc2d;
	AcGeCircArc2d* m_geArc2dLeftTop;
	AcGeCircArc2d* m_geArc2dLeftBot;
	AcGeCircArc2d* m_geArc2dRightTop;
	AcGeCircArc2d* m_geArc2dRightBot; 
	vector<AcGeCircArc2d*> m_arcs;
private:
	bool breakArcIntoTwoPartBy(IN double dblAngelToBreak,IN AcGeCircArc2d* pGeArc2d,OUT AcGeCircArc2d* pArcResult1,OUT AcGeCircArc2d* pArcResult2);

public:
	bool inputArcSeg(AcGeCircArc2d* pGeArc2d); 
};



/*
class CBreakAcGeCircArcToTwoPart
功能：将输入的弧，按输入的角度，打破成2段弧;
输入：弧段，角度;
输出：打破后的2段弧;
*/
class CBreakAcGeCircArcToTwoPart
{
private:
	AcGeCircArc2d* m_inputArc;
	double m_inputRadianToBreak;  
	AcGeCircArc2d* m_arcResult1; 
	AcGeCircArc2d* m_arcResult2; 
	bool isBreakSuccessed;
public:
	bool outputBreakedArcs(OUT AcGeCircArc2d* pArcResult1,OUT AcGeCircArc2d* pArcResult2);
	bool breakArc(IN AcGeCircArc2d* pArc,IN double radianToBrk,OUT AcGeCircArc2d* pArcResult1,OUT AcGeCircArc2d* pArcResult2); 
	bool breakArc();
	bool setArcToBeBrked(IN AcGeCircArc2d* pArc); 
	bool setRadianToBeBrked(IN double radianToBrk);  
	bool setArcAndRadianToBreak(IN AcGeCircArc2d* pArc,IN double radianToBrk);   

public:
	CBreakAcGeCircArcToTwoPart();  
	~CBreakAcGeCircArcToTwoPart();  
};

#endif  //ZHOUHUAGANG_20190822_segments_intersections