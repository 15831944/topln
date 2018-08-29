//************************************************************
//Copyright (C), 2018-8-14, owal Tech. Co., Ltd.
//FileName: findTheGapBtwPoints.cpp
//Author: Version : Date:
//Description: 
//�������С��dist�ĵ�ԣ�Ҫ��
//1����Բ�����ͬһ���߶εĶ˵㣻
//2����Բ����غ�;
//3�����֮�������û���߶�������;
//Version: // �汾��Ϣ;
//Function List: // ��Ҫ�������书��
//1. -------
//History: // ��ʷ�޸ļ�¼
//<author> <time> <version > <desc>
//owal  2018-08-14  1.0 build this moudle
//***********************************************************/
#include "..\StdAfx.h"
#include "TCHAR.h"
#include "findTheGapBtwPoints.h"



//=======================
//class CFindGapBtwPoints
//1.����С��ָ������dist�ĵ�ԣ�Ҫ���Բ��غ�;
//2.Χ�Ʋ��ҵ��ĵ�ԣ���һЩ����;
//=======================
CFindGapBtwPoints::CFindGapBtwPoints()
{	
}


//
CFindGapBtwPoints::~CFindGapBtwPoints()
{
}


//��һ��ֱ�߶Σ���������������;
void
CFindGapBtwPoints::addOneLine(const AcGePoint3d pt1,const AcGePoint3d pt2)
{
	AcDbLine* pLine = new AcDbLine;
	pLine->setStartPoint(pt1);
	pLine->setEndPoint(pt2);
	join2database(pLine);
	pLine->close();
	//Ҫdelete�𣿲���delete pLine;
}


//
void
CFindGapBtwPoints::addAllLine()
{
	;
}


//zoom the window created by two points;
void
CFindGapBtwPoints::zoomArea(const AcGePoint3d pt1,const AcGePoint3d pt2)
{
	//������ڷŴ�����Ҫ�ԷŴ�Χ���д���;
	AcEdCommand(RTSTR,_T("zoom"),RTPOINT,pt1,RTPOINT,pt2,0);   
}


//ȡ��ֱ�߶ε������˵�
//retun: true-�ɹ��� false-ʧ��;
bool 
getLineEndPoints(IN const AcDbLine* linePtr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte)
{
	if(linePtr == NULL)
	{
		return false;
	}

	pts = linePtr->startPoint();
	pte = linePtr->endPoint();

	return true;
}


//ȡ�û��ε������Ķ˵㣻�������뵽�㼯��;
//retun: true-�ɹ��� false-ʧ��;
bool 
getArcEndPoints(const AcDbArc* arcptr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte)
{
	if(arcptr == NULL)
	{
		return false; 
	}

	//���㻡�������˵�; ����AcGeVector2d�࣬�ȽϷ���;
	AcGePoint3d ptCenter = arcptr->center();
	double dblRadiu = arcptr->radius();
	AcGeVector2d vectpt;
	vectpt.set(dblRadiu,0); 
	double startAngle = arcptr->startAngle();
	double endAngle = arcptr->endAngle();
	vectpt.rotateBy(startAngle);
	pts.set(ptCenter.x + vectpt.x,ptCenter.y + vectpt.y,0);
	vectpt.rotateBy(endAngle - startAngle)); 
	pte.set(ptCenter.x + vectpt.x, ptCenter.y + vectpt.y,0);  
	
	return true;
}


//ȡ�ö����ߵ������˵�;�������뵽�㼯;
//retun: true-�ɹ��� false-ʧ��;
bool 
CFindGapBtwPoints::getPolylineEndPoints(const AcDbPolyline* plPtr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte)
{
	int nNumVtx = 0;
	plPtr->
}