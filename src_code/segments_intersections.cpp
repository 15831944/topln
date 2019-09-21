/************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: Version :
Date:
Description:  1、line,arc轮廓提取；

Version: // 版本信息;
Function List: // 主要函数及其功能
1. 
2.
3.
History: // 历史修改记录
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/


#include "segments_intersections.h"



/*------------------------------------------------------------
class CSegement
------------------------------------------------------------*/

//class CSegement::initCSegement
//功能：1.建立线段：从line，arc，circle，polyline提取线段;
//2.初始化Csegement;
bool
CSegement::initCSegement(IN AcDbLine* linePtr)
{
	double x1;
	double y1;
	double x2;
	double y2;
	x1 = linePtr->startPoint().x;
	y1 = linePtr->startPoint().y;
	x2 = linePtr->endPoint().x;
	y2 = linePtr->endPoint().y;
	s
	//m_x赋值
	if(y1 > y2)
	{
		m_xTop = x1;
		m_yTop = y1;
		m_xBottom = x2;
		m_yBottom = y2;
	}
	else
	{
		m_xTop = x2;
		m_yTop = y2;
		m_xBottom = x1;
		m_yBottom = y2;
	}

	//self
	m_myselfType = Line_Type;
	m_arcPtr = NULL;
	m_circlePtr = NULL;
	m_LinePtr = linePtr;

	//mather
	m_parentType =  None_Type;
	m_arcPtr = NULL;
	AcDbCircle* m_circlePtr = NULL;
	//AcDbLine* m_LinePtr;
	AcGeCircArc2d* m_circArc2d = NULL;
	AcDbPolyline* m_polylinePtr = NULL;

	//grandmather
	ETypeOfArc m_grandmaType = None_Type;
	//AcDbArc* m_arcPtr;
	//AcDbLine* m_LinePtr;
	AcDbPolyline* m_polylinePtr = NULL;
	
	return true;
}




/*------------------------------------------------------------
class CArcToSegment
------------------------------------------------------------*/
/*
CArcToSegment::breakArcIntoTwoPartBy()
*/
bool CArcToSegment::breakArcIntoTwoPartBy(IN double dblAngelToBreak,IN AcGeCircArc2d* pGeArc2d,OUT AcGeCircArc2d* pArcResult1,OUT AcGeCircArc2d* pArcResult2)
{
	if(pGeArc2d == NULL)
	{
		return false;
	}

	double startAngle = m_geArc2d->startAng(); 
	double endAngle = m_geArc2d->endAng(); 
	double radius = m_geArc2d->radius();
	//AcGeVector3d vec3d = m_geArc2d->refVec();
	
	;
}


/*-----------------------------------------------
class CBreakAcGeCircArcToTwoPart
功能：按给定的弧度，把一段弧打碎为2段；
-----------------------------------------------*/
CBreakAcGeCircArcToTwoPart::CBreakAcGeCircArcToTwoPart()
{
}

<<<<<<< HEAD
CBreakAcGeCircArcToTwoPart::~CBreakAcGeCircArcToTwoPart()
{
}
=======
/*
CArcToSegment
功能：把弧段切分成四份，每个象限一份;
*/
>>>>>>> 37dd28f380eb75c30be740c2401645f6f951599b

//initialize the member variables;
bool
CBreakAcGeCircArcToTwoPart::setArcAndRadianToBreak(IN AcGeCircArc2d* pArc,IN double radianToBrk)
{
	bool b1 = false;
	bool b2 = false;
	b1 = setArcToBeBrked(pArc);
	b2 = setRadianToBeBrked(radianToBrk);
	return (b1 && b2);
}


//initialize
bool
CBreakAcGeCircArcToTwoPart::setArcToBeBrked(IN AcGeCircArc2d* pArc)
{
	m_inputArc = pArc;
	if(pArc == NULL)
	{		
		return false;
	}
	else
	{	
		return true;
	}
}

<<<<<<< HEAD

//initialize
//误差值以AcGeTol::equalPoint()为准;
bool
CBreakAcGeCircArcToTwoPart::setRadianToBeBrked(IN double radianToBrk)
{
	m_inputRadianToBreak = radianToBrk;
	if((radianToBrk < -AcGeTol::equalPoint()) && (radianToBrk >= (2*Pi -AcGeTol::equalPoint())))
	{		
		return false;
	}
	else
	{	
		return true;
	}
}


//break the input arc
bool
CBreakAcGeCircArcToTwoPart::breakArc()
{
	double startAng = m_inputArc->startAng();
	double endAng = m_inputArc->endAng();
	double radius = m_inputArc->radius();
}


//output the result arcs
bool
CBreakAcGeCircArcToTwoPart::outputBreakedArc(OUT AcGeCircArc2d* pArcResult1,OUT AcGeCircArc2d* pArcResult2)
{
	;
}

=======
	AcGeTol tol;

	double startAngle = m_geArc2d->startAng();   
	double endAngle = m_geArc2d->endAng();
	double radius = m_geArc2d->radius();
	AcGeVector2d vec2d = m_geArc2d->refVec();
	Adesk::Boolean isclockwise = m_geArc2d->isClockWise();
	
	bool isZeroIN = false;
	bool isNintyIn = false;
	bool is270In = false;
	bool is180In = false;
	//判断其实角度和结束角度的关系:0,90,180,270度可否插入; 
	//0度可插入吗？
	if(startAngle < 0 - tol.equalPoint() && endAngle > 0 + tol.equalPoint())
	{
		isZeroIN = true;
	}
}
>>>>>>> 37dd28f380eb75c30be740c2401645f6f951599b
