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
功能：把弧段切分成四份，每个象限一份;
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


CBreakAcGeCircArcToTwoPart::~CBreakAcGeCircArcToTwoPart()
{
}



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
	AcGePoint2d centerPt = m_inputArc->center(); 

	//判断角度m_inputRadianToBreak是否在弧内;
	if(m_inputArc > (startAng + AcGeTol::equalPoint())) 
	{ 
		if(m_inputArc < (endAng - AcGeTol::equalPoint()))  
		{
			m_arcResult1->setRadius(radius);
			m_arcResult1->setAngles(startAng,m_inputArc);  
			m_arcResult1->setCenter(centerPt);

			m_arcResult2->setRadius(radius);
			m_arcResult2->setAngles(m_inputArc,endAng);
			m_arcResult2->setCenter(centerPt);

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}



//output the result arcs 
bool
CBreakAcGeCircArcToTwoPart::outputBreakedArcs(OUT AcGeCircArc2d* pArcResult1,OUT AcGeCircArc2d* pArcResult2)
{
	bool b1 = false;
	bool b2 = false;
	if(m_arcResult1 != NULL)
	{
		pArcResult1 = m_arcResult1;
		b1 = true;
	}

	if(m_arcResult2 != NULL)
	{
		pArcResult2 = m_arcResult2;
		b2 = true;
	}

	return (b1 || b2);
}


//
bool 
CBreakAcGeCircArcToTwoPart::breakArc(IN AcGeCircArc2d* pArc,IN double radianToBrk,OUT AcGeCircArc2d* pArcResult1,OUT AcGeCircArc2d* pArcResult2)
{
	if(!setArcToBeBrked(pArc))
	{
		return false;
	}

	if(!setRadianToBeBrked(radianToBrk))
	{
		return false;
	}

	if(!breakArc())
	{
		return false;
	}

	if(!outputBreakedArcs(pArcResult1,pArcResult2))
	{
		return false;
	}

	return true;
}


