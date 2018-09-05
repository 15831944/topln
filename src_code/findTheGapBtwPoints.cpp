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
	//�ͷ��ڴ�
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
	acedCommand(RTSTR,_T("zoom"),RTPOINT,pt1,RTPOINT,pt2,0);   
}


//ȡ��ֱ�߶ε������˵�
//retun: true-�ɹ��� false-ʧ��;
bool 
CFindGapBtwPoints::getLineEndPoints(IN const AcDbLine* linePtr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte)
{
	if(linePtr == NULL)
	{
		return false;
	}

	pts = linePtr->startPoint();
	pte = linePtr->endPoint();
	//entId = linePtr->objectId();

	return true;
}


//ȡ�û��ε������Ķ˵㣻�������뵽�㼯��;
//retun: true-�ɹ��� false-ʧ��;
bool 
CFindGapBtwPoints::getArcEndPoints(const AcDbArc* arcptr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte)
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
	vectpt.rotateBy(endAngle - startAngle); 
	pte.set(ptCenter.x + vectpt.x, ptCenter.y + vectpt.y,0);  

	//entId = arcptr->objectId();
	
	return true;
}


//ȡ�ö����ߵ������˵�;�������뵽�㼯;
//retun: true-�ɹ��� false-ʧ��;
bool 
CFindGapBtwPoints::getPolylineEndPoints(const AcDbPolyline* plPtr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte)
{
	if(plPtr->isClosed())
	{
		return false; //�������Ǳպϵģ�û��Ҫ����ȡ�˵�;
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
	//	return false; //������ȣ��򲻷��أ�
	//}
	return true;
}


//�ж��������Ƿ���ȣ��ص�;
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


//��ȡʵ�弯��
bool 
CFindGapBtwPoints::inputAdsName(IN ads_name ss)
{
	if(acdbNameNil(ss))
	{
		return false;
	}
	else
	{
		acdbNameSet(ss,m_ssall);
		return true;
	}
	
}


//1��ȡ�û��������С����dist��dist����������Ҫ��ȡ�ĵ��;
//2���˾����Ƿ�Ϸ�������Ϊ��������С��cad����Сֵ��
//���أ�����Ϸ�������true�� ����false;
bool 
CFindGapBtwPoints::inputMinDistByUser(IN const double minDist) 
{
	AcGeTol objTol;
	double tmpDist = objTol.equalPoint();
	if(minDist <= tmpDist)
	{
		m_dist = minDist;
		acutPrintf(_T("\n the min dist is too small,exit!"));
		return false;  
	}
	else
	{
		return true;
	}
}


void
CFindGapBtwPoints::getPointPair(OUT vector<pair<void*,void*>>& vPointPairs) 
{
	vPointPairs = m_vPointPairs; //��ֵһ��;   
}


//���ܣ�1���û�ѡ��ʵ�弯�ϣ�   
//      2���ռ����ص��ĵ�;
//      3������С���û�����ľ���ĵ�ԣ��ҵ�Բ���ͬһ�߶���;   
//���أ�void;
void
CFindGapBtwPoints::parsePointPairs() 
{
	//���ʵ������;  
	ads_name ssUnit; //����ʵ��;  
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

	//��ʵ����ȡ�˵�;  
	//���˵����㼯;
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

	//��ȡ������;
	m_pointsMap.findPointPairs(m_dist,m_vPointPairs);  
	
	//return; 
}


//extract the end points from the arc
//���أ�ȡ���˵㣬����true�� ���򷵻�false;   
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

	//�ж�ʵ������
	if(pEnt->isA() == AcDbPolyline::desc()) 
	{
		if(getPolylineEndPoints((AcDbPolyline*)pEnt,pts,pte))
		{
			pEnt->close();
			entId = pEnt->objectId();
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
		if(getLineEndPoints((AcDbLine*)pEnt,pts,pte))
		{
			pEnt->close();
			entId = pEnt->objectId();
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
		if(getArcEndPoints((AcDbArc*)pEnt,pts,pte))
		{
			pEnt->close();
			entId = pEnt->objectId();
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


//��������굽�㼯; 
void
CFindGapBtwPoints::insertPoints(IN const AcGePoint3d pt3d,IN const AcDbObjectId objId)
{
	SAttachData* pAttData = new SAttachData;
	m_vAttDataPtrs.push_back(pAttData);
	pAttData->m_pt3d = pt3d;
	pAttData->m_entId = objId; 
	m_pointsMap.insert(pt3d,0,pAttData);     
}


//===========================
//����ͼ����С���û�ָ����������е��
//��Ҫ����ģ��
//Ҳ�ǲ���ģ��
//===========================
void
findPtPairDistLessThan()
{
	//ѡ��ʵ�弯
	ads_name ss;
	CSelectEnts objSel;
	acdbNameClear(ss); //����Ϊnil;  
	objSel.usrSelect(ss); 

	vector<pair<void*,void*>> vAllPointPair;   
	CFindGapBtwPoints objFindPointPairs;   
	objFindPointPairs.inputAdsName(ss);   
	objFindPointPairs.getPointPair(vAllPointPair);    

	//��Թ���;
	//����������; ֻ��һ�����Ҫ���˵����������ͬһ��ֱ�߶�;

	//��ӡ���;
	;
}