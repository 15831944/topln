/************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: Version :
Date:
Description:  1��line,arc������ȡ��

Version: // �汾��Ϣ;
Function List: // ��Ҫ�������书��
1. 
2.
3.
History: // ��ʷ�޸ļ�¼
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/


#include "segments_intersections.h"



/*
class CSegement::initCSegement
���ܣ�1.�����߶Σ���line��arc��circle��polyline��ȡ�߶�;
      2.��ʼ��Csegement;
*/
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
	//m_x��ֵ
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


/*
�Ӷ�����ĳ��(��֪�ǻ�)����ȡ���ε�ĳ���֣����Ĳ��֣�;

*/
bool
CSegement::initCSegement(IN AcDbPolyline* polylinePtr,IN int indexVertex,IN int arcPartNumber)
{
	;
}



/*
CArcToSegment
*/

bool CArcToSegment::getSegment()
{
	if(m_geArc2dLeftTop == NULL)
	{
		return false;
	}

	double startAngle = m_geArc2d->startAng();   
	double endAngle = m_geArc2d->endAng();
	double radius = m_geArc2d->radius();
	AcGeVector3d vec3d = m_geArc2d->
	;
}