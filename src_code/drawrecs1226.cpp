/************************************************************
Copyright (C), 2016-6-12, XuMiJieZi.
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
#include "..\StdAfx.h"
#include "drawrecs1226.h"
#include "TCHAR.h"


//=============================
//class CDrawRects
//=============================

CDrawRects::CDrawRects()
{
}


CDrawRects::~CDrawRects()
{
}


//判断输入点是否可以画正方形;
//判断其右上顶点坐标是否存在;
bool
CDrawRects::isPointRight(IN const AcGePoint3d pt3d)
{
	int ptIndex = -1;
	AcGePoint3d ptRT;
	ptRT.set(pt3d.x + 1,pt3d.y+1,0);
	return m_ptMap.find(ptRT,ptIndex);
}


//将输入点坐标作为左下角顶点，画一个正方形，边长为1;
void
CDrawRects::drawRects(IN const AcGePoint3d pt3d)
{
	AcGePoint2d pt2d;
	pt2d.set(pt3d.x,pt3d.y);

	AcDbPolyline* pPline;
	AcGePoint2d pt2; //right bottom;
	AcGePoint2d pt3;  //right top;
	AcGePoint2d pt4;
	
	pt2.set(pt3d.x + 1,pt3d.y);
	pt3.set(pt3d.x + 1,pt3d.y + 1);
	pt4.set(pt3d.x,pt3d.y + 1);
	double fbugle = 0;
	double dblWidth = 0;

	pPline = new AcDbPolyline;
	pPline->addVertexAt(0,pt2d,fbugle,dblWidth,dblWidth);
	pPline->addVertexAt(0,pt2,fbugle,dblWidth,dblWidth);
	pPline->addVertexAt(0,pt3,fbugle,dblWidth,dblWidth);
	pPline->addVertexAt(0,pt4,fbugle,dblWidth,dblWidth);
	pPline->setClosed(Adesk::kTrue);

	m_allPlines.push_back(pPline);  //收集起来;
}


//将多义线加入数据库;
void
CDrawRects::join2db()
{
	//create new layer
	addLayer(_T("soManyRects"));

	AcDbPolyline* pPline = NULL;

	vector<AcDbPolyline*>::iterator itr = m_allPlines.begin();
	for(; itr != m_allPlines.end(); itr++)
	{
		pPline = (AcDbPolyline*)(*itr);
		pPline->setLayer(_T("soManyRects"));
		join2database(pPline);
		pPline->close();
	}
}


//提取所选实体的顶点;
//只针对line 和 arc;
void
CDrawRects::collectPoints(ads_name ss)
{
	ads_name ssUnit; 

	long nLen = 0;
	acedSSLength(ss,&nLen);
	if(nLen <= 0)
	{
		return;
	}

	AcDbObjectId id;
	AcDbEntity* pEnt;
	for(long i = 0; i < nLen; i++)
	{
		acedSSName(ss,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);
		if(pEnt->isA() == AcDbLine::desc())
		{
			collectPointsOfLine(pEnt);
		}
		else if(pEnt->isA() == AcDbPolyline::desc())
		{
			collectPointsOfPolyline(pEnt);
		}
		else if(pEnt->isA() == AcDbArc::desc())
		{
			collectPointsOfArc(pEnt);
		}
		pEnt->close();
	}

	acutPrintf(_T("\n======================"));
	m_ptMap.print();
	acutPrintf(_T("\n======================"));
}


//collect line points
void
CDrawRects::collectPointsOfLine(IN const AcDbEntity*  pEnt)
{
	if(pEnt->isA() != AcDbLine::desc()) 
	{
		return;
	}

	//
	AcGePoint3d pt1;
	AcGePoint3d pt2;
	AcDbLine* pLine = (AcDbLine*)pEnt;
	
	pt1 = pLine->startPoint();
	pt2 = pLine->endPoint();

	int ptIndex = 0;
	if(!m_ptMap.find(pt1,ptIndex))
	{
		m_ptMap.insert(pt1,0);
		m_allPoints.push_back(pt1);
	}

	if(!m_ptMap.find(pt2,ptIndex))
	{
		m_ptMap.insert(pt2,0);
		m_allPoints.push_back(pt2);
	}
}


//collect line points
void
CDrawRects::collectPointsOfArc(IN const AcDbEntity* pEnt)
{
	if(pEnt->isA() != AcDbArc::desc()) 
	{
		return;
	}
	AcDbArc* pArc = (AcDbArc*)pEnt;

	//pt1,pt2
	AcGePoint3d ptCenter;
	AcGePoint3d pt1;
	AcGePoint3d pt2;
	AcGeVector2d vec1;
	ptCenter = pArc->center();
	double dblRadius = pArc->radius();
	vec1.set(dblRadius,0);
	double angStart = pArc->startAngle(); 
	double angEnd = pArc->endAngle();
	vec1.rotateBy(angStart);
	pt1.set(ptCenter.x + vec1.x,ptCenter.y + vec1.y,0); // pt1;
	vec1.set(dblRadius,0);
	vec1.rotateBy(angEnd);
	pt2.set(ptCenter.x + vec1.x,ptCenter.y + vec1.y,0);  //pt2;

	//join to map
	int ptIndex = 0;
	if(!m_ptMap.find(pt1,ptIndex))
	{
		m_ptMap.insert(pt1,0);
		m_allPoints.push_back(pt1);
	}

	if(!m_ptMap.find(pt2,ptIndex))
	{
		m_ptMap.insert(pt2,0);
		m_allPoints.push_back(pt2);
	}
}


//collect line points
void
CDrawRects::collectPointsOfPolyline(IN const AcDbEntity* pEnt)
{
	if(pEnt->isA() != AcDbPolyline::desc()) 
	{
		return;
	}
	
	//
	int ptIndex = 0;
	AcGePoint3d pt1;
	AcDbPolyline* pPline = (AcDbPolyline*)pEnt;
	int nNumVerts = pPline->numVerts();
	for(int i = 0; i < nNumVerts; i++)
	{
		pPline->getPointAt(i,pt1);
		if(!m_ptMap.find(pt1,ptIndex))
		{
			m_ptMap.insert(pt1,0);
			m_allPoints.push_back(pt1);
		}
	}	
}


//画所有正方形;
//查找其右上顶点是否存在，存在就画正方形;
void
CDrawRects::drawAllRects()
{
	int ptIndex = 0;
	AcGePoint3d pt1;
	AcGePoint3d pt2;
	vector<AcGePoint3d>::iterator itr = m_allPoints.begin();
	for(; itr != m_allPoints.end(); itr++)
	{
		pt1 = (AcGePoint3d)(*itr);
		pt2.set(pt1.x + 1,pt1.y+1,0);		
		if(m_ptMap.find(pt2,ptIndex))
		{
			drawRects(pt1);
		}
		else
		{
			//acutPrintf(_T("   not find.\n"));
		}
	}

	//join to database;
	AcDbPolyline* pPline;
	vector<AcDbPolyline*>::iterator itra = m_allPlines.begin();
	for(; itra != m_allPlines.end(); itra++)
	{
		pPline = (AcDbPolyline*)(*itra);
		join2database(pPline);
		pPline->close();
	}
}


void
CDrawRects::doit()
{
	ads_name ss;
	if(!select(ss))
	{
		return;
	}

	//收集所有的点;
	collectPoints(ss);

	//对所有的点画正方形;
	drawAllRects();

	acedSSFree(ss);
}



//drects command
void
drawrects()
{
	CDrawRects objDr;
	objDr.doit();
}
