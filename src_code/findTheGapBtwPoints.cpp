//************************************************************
//Copyright (C), 2018-8-14, owal Tech. Co., Ltd.
//FileName: findTheGapBtwPoints.cpp
//Author: Version : Date:
//Description: 
//计算距离小于dist的点对；要求：
//1、点对不能是同一个线段的端点；
//2、点对不能重合;
//3、点对之间必须是没有线段相连的;
//Version: // 版本信息;
//Function List: // 主要函数及其功能
//1. -------
//History: // 历史修改记录
//<author> <time> <version > <desc>
//owal  2018-08-14  1.0 build this moudle
//***********************************************************/
#include "..\StdAfx.h"
#include "TCHAR.h"
#include "findTheGapBtwPoints.h"



//=======================
//class CFindGapBtwPoints
//1.查找小于指定距离dist的点对，要求点对不重合;
//2.围绕查找到的点对，做一些功能;
//=======================
//constructor
CFindGapBtwPoints::CFindGapBtwPoints()
{
	m_dist = 0;
	acdbNameClear(m_ssall);
	m_pEnt = NULL;	
}


//
CFindGapBtwPoints::~CFindGapBtwPoints()
{
	//释放内存
	SAttachData* ptrAtt;
	vector<SAttachData*>::iterator itrAtt = m_vAttDataPtrs.begin();
	for(;itrAtt != m_vAttDataPtrs.end();itrAtt++)
	{
		ptrAtt = (SAttachData*)(*itrAtt);
		if(ptrAtt != NULL)
		{
			delete ptrAtt;
		}
	}
}


//画一条直线段；用来连接两个点;
void
CFindGapBtwPoints::addOneLine(const AcGePoint3d pt1,const AcGePoint3d pt2)
{
	AcDbLine* pLine = new AcDbLine;
	pLine->setStartPoint(pt1);
	pLine->setEndPoint(pt2);
	join2database(pLine);
	pLine->close();
	//要delete吗？不用delete pLine;
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
	//如果过于放大，则需要对放大范围进行处理;
	acedCommand(RTSTR,_T("zoom"),RTPOINT,pt1,RTPOINT,pt2,0);   
}


//取得直线段的两个端点
//retun: true-成功； false-失败;
bool 
CFindGapBtwPoints::getLineEndPoints(IN const AcDbLine* linePtr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte)
{
	if(linePtr == NULL)
	{
		return false;
	}

	pts = linePtr->startPoint();
	pte = linePtr->endPoint();

	return true;
}


//取得弧段的两个的端点；用来插入到点集里;
//retun: true-成功； false-失败;
bool 
CFindGapBtwPoints::getArcEndPoints(const AcDbArc* arcptr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte)
{
	if(arcptr == NULL)
	{
		return false; 
	}

	//计算弧的两个端点; 用了AcGeVector2d类，比较方便;
	AcGePoint3d ptCenter = arcptr->center();
	double dblRadiu = arcptr->radius();
	AcGeVector2d vectpt;
	vectpt.set(dblRadiu,0); 
	double startAngle = arcptr->startAngle();
	double endAngle = arcptr->endAngle();
	vectpt.rotateBy(startAngle);
	pts.set(ptCenter.x + vectpt.x,ptCenter.y + vectpt.y,0);
	vectpt.rotateBy(endAngle - startAngle); 
	pte.set(ptCenter.x + vectpt.x, ptCenter.y + vectpt.y,0);  
	
	return true;
}


//取得多义线的两个端点;用来插入到点集;
//retun: true-成功； false-失败;
bool 
CFindGapBtwPoints::getPolylineEndPoints(const AcDbPolyline* plPtr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte)
{
	if(plPtr->isClosed())
	{
		return false; //多义线是闭合的，没必要继续取端点;
	}
	int nNumVtx = 0;
	nNumVtx = plPtr->numVerts();
	if(nNumVtx == 0)
	{
		return false;
	}

	plPtr->getPointAt(0,pts);
	plPtr->getPointAt(nNumVtx -1,pte);
	//if(pts.isEqualTo(pte))
	//{
	//	return false; //两点相等，则不返回？
	//}
	return true;
}


//判断两个点是否相等，重叠;
bool
CFindGapBtwPoints::isTwoPointsOverlapped(IN AcGePoint3d& pts,IN AcGePoint3d& pte)
{
	if(pts.isEqualTo(pte))
	{
		return true;
	}
	else
	{
		return false;
	}
}


//1获取用户输入的最小距离dist，dist用来计算需要获取的点对;
//2检查此距离是否合法：必须为正，不能小于cad的最小值；
//返回：距离合法，返回true； 否则false;
bool 
CFindGapBtwPoints::inputMinDistByUser(const double minDist)
{
	AcGeTol objTol;
	double tmpDist = objTol.equalPoint();
	if(minDist <= tmpDist)
	{
		minDist = tmpDist;
		return false;
	}
	else
	{
		return true;
	}
}


//功能：1、用户选择实体集合；
//      2、收集不重叠的点;
//      3、查找小于用户输入的距离的点对，且点对不在同一线段上;
//返回：void;
void
CFindGapBtwPoints::getPointPair(vector<pair<void*,void*>>& m_vPointPairs)
{
	//选择实体集
	CSelectEnts objSel;
	acdbNameClear(m_ssall); //设置为nil;
	objSel.usrSelect(m_ssall);
	if(acdbNameNil(m_ssall))
	{
		return;
	}

	//检测实体数量;  
	ads_name ssUnit; //单个实体;
	long nLen = 0; 
	if(RTNORM != acedSSLength(m_ssall,&nLen))
	{
		return;
	}
	else
	{
		if(nLen <= 0)
		{
			return;
		}
	}

	//对实体提取端点;  
	//将端点插入点集;
	AcGePoint3d pts;
	AcGePoint3d pte;
	AcDbObjectId objId;
	for(long i = 0; i < nLen; i++)
	{
		acedSSName(m_ssall,i,ssUnit);
		if(extrPntsFromEntity(ssUnit,pts,pte,objId))
		{
			insertPoints(pts,objId);
			insertPoints(pte,objId);
		}
	}

	//获取最近点对;
	m_pointsMap.findPointPairs(m_vPointPairs);
	//return;
}


//extract the end points from the arc
//返回：取到端点，返回true； 否则返回false;
bool
CFindGapBtwPoints::extrPntsFromEntity(const ads_name ssUnit,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte,OUT AcDbObjectId& entId) 
{
	AcDbObjectId objId;
	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus errSts;
	errSts = acdbOpenObject(pEnt,objId,AcDb::kForRead);
	if(errSts != Acad::eOk)
	{
		pEnt->close();
		return false;
	}

	//判断实体类型
	if(pEnt->isA() == AcDbPolyline::desc()) 
	{
		if(getPolylineEndPoints((AcDbPolyline*)pEnt,pts,pte,entId))
		{
			pEnt->close();
			return true;
		}
		else
		{
			pEnt->close();
			return false;
		}
	}
	else if(pEnt->isA() == AcDbLine::desc())
	{
		if(getLineEndPoints((AcDbLine*)pEnt,pts,pte,entId))
		{
			pEnt->close();
			return true;
		}
		else
		{
			pEnt->close();
			return false;
		}
	}
	else if(pEnt->isA() == AcDbArc::desc()) 
	{
		if(getArcEndPoints((AcDbArc*)pEnt,pts,pte,entId))
		{
			pEnt->close();
			return true;
		}
		else
		{
			pEnt->close();
			return false;
		}
	}
	else
	{
		pEnt->close();
		return false;
	}
}


//插入点坐标到点集;
void
CFindGapBtwPoints::insertPoints(IN const AcGePoint3d pt3d,IN const AcDbObjectId objId)
{
	SAttachData* pAttData = new SAttachData;
	m_vAttDataPtrs.push_back(pAttData);
	pAttData->m_pt3d = pt3d;
	pAttData->m_entId = objId;
	m_pointsMap.insert(pt3d,0,pAttData);
}