//*************************************************************************
//Copyright (C), 2018-08-14, ���ֽ���  Mountain & Sesame Tech. Co., Ltd.
//File name:  findTheGapBtwPoints.h
//Author: 
//Version:
//Date: 2018-08-14
//Description: 
//��cpp�ļ�˵��;
//Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵�� 
//1. ....
//History: 
//�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ���
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
//���ҵ�Ե�Ҫ��
//1.���ص��ĵ�; (CPointMap���Ѿ������ص���)
//2.��Ծ��벻����ָ������list;
//3.����ͬһ�߶���;
//���֮�䲻����·��;
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
	bool isTwoPointsOverlapped(IN AcGePoint3d& pts,IN AcGePoint3d& pte);  //�ж��������Ƿ��غ�(���);  
	void insertPoints(IN const AcGePoint3d pt3d,IN const AcDbObjectId objId); 
	bool extrPntsFromEntity(const ads_name ssUnit,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte,OUT AcDbObjectId& objId); 
	void parsePointPairs();  //���㲻����һ������ĵ��;
	void filterPointPairs();  //���˵��;�����ϵĵ��ɾ����;  

public:
	bool inputAdsName(IN ads_name ss); 
	bool inputMinDistByUser(const double minDist);
	//void getPickSet(ads_name ss);    //��ʾ�û�ѡȡʵ�弯;   
	void getPointPair(OUT vector<pair<void*,void*>>&  vPointPairs);  	
	void addOneLine(const AcGePoint3d pt1,const AcGePoint3d pt2);  //connect the gap;    
	void addAllLine();  //ȫ����������; 
	void zoomArea(const AcGePoint3d pt1,const AcGePoint3d pt2);  //zoom by two points;   

private:
	CPointMap m_pointsMap;  //�洢������; 
	ads_name m_ssall;  //�û�ѡ��ġ����˺��ʵ�弯��;    
	AcDbEntity* m_pEnt; 
	vector<pair<void*,void*>> m_vPointPairs;    
	vector<SAttachData*> m_vAttDataPtrs;  //gathered to release the memory;    
	double m_dist;  //�û����������룬С�ڴ˾���ĵ�Ա��������; 
};


#endif //OWIN_FINDTHEGAPBTWPOINTS_H_   