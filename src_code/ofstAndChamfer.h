/*************************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: ofstAndChamfer.h
Author: 
Version:
Date: // hugo��1.0
Description: // ��ͼ����arc��line��polyline��ȡ: 
Others: // �������ݵ�˵��
        1.0 ��ģ�����ϣ�ֻ����Ϊ�ο����㷨��������Ҳ������. 2017-9-7 �ܻ���;
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
eg�� Date:	Author: 	Modification:
 dec-13 2017 owal
*************************************************************************/
#ifndef  ZHOUHUAGANG_OFSTANDCHAMFER_H
#define  ZHOUHUAGANG_OFSTANDCHAMFER_H



#include <vector>
#include <algorithm>


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

#include  "opt.h"
#include "elapsetime.h"
#include "basefunc.h"


//����ofstpl
void ofstPline();
void testIfCCWPline(); //old version,give up;
void testPlineCCW();  

//�������Ƿ�պ�;
enum EIsPlineClosed
{
	REAL_CLOSED,   //��ıպ�
	ASSUME_CLOSED,  //�����յ��غϣ����Ƕ�����û��closed;
	NOT_CLOSED      //�����յ㲻�غ�;
};

enum EIsPlineCCW
{
	CLOCKWISE_PLINE,
	UNCLOCKWISE_PLINE,
	ERROR_PLINE
};




//��Զ����ߵĶ�����
//�÷������û�����ƫ��ֵ; Ȼ���Զ�offset
//ֻ��Ապ϶����߼�Բʵ��;
class CPlineOffset
{
public:
	CPlineOffset();
	~CPlineOffset();

private: //member var;   
	double m_offsetDist;  //offset distance;    
	//AcDbVoidPtrArray m_plineArr;  //ʢ��offset���polyline;    
public:
	void ofst(IN ads_name ss,OUT AcDbVoidPtrArray& m_plineArr);  //�Զ�����offset;     
	void ofst(IN AcDbPolyline* pPlineOrg, OUT AcDbVoidPtrArray& m_plineArr);  
	void ofst(IN AcDbCircle* pCircle);
	double setOffsetDist(IN const double dblDist);  //offset value;   
	EIsPlineCCW isPlineCCW(IN AcDbPolyline* pline);  //�ж϶������Ƿ���ʱ��;    
private:	
	bool checkPolyline(IN const AcDbEntity* pEnt);   
	bool checkOffsetRsltSize(IN const AcDbExtents orgExt,IN const AcDbExtents newExt,IN const double dblOfstSize);  
};



#define  VECTOR_TOL (0.000001)
//===========================
//���·�����֤���԰��������Ч;��Ч��Ч��Ч��Ч��Ч��Ч��Ч*��Ч*��Ч*��Ч*��Ч*��Ч*��Ч!
//�����͹����Σ�Ҳ����Ҫ����ô����;
//===========================
//class CIsPlineCCW
//�ж�һ���������Ƿ���ʱ�룻���ֻ�Ապ϶�������Ч;
//��������߲��պ�,�жϽ������Ч��;
class CIsPlineCCW
{
private:
	vector<AcGeVector3d> m_allVec3d; //�ռ�����vector3d;
	EIsPlineClosed m_isClosed;  //�������Ƿ�պ�;
	AcDbPolyline* m_pline;
	int m_nNumVerts;   //�����߶�����;
	vector<double> m_crossProductNum;  //ÿ�����㴦�������ߴ��������Ĳ����zֵ; 
	vector<double> m_anglesLeft;  //m_allAngles������;
	const double m_tol;  //�ж������Ƿ�ƽ�е����ֵ;
public:
	CIsPlineCCW();
	CIsPlineCCW(IN  AcDbPolyline* pPline);
	~CIsPlineCCW();
public:	
	EIsPlineClosed isClosed(IN const AcDbPolyline* pline);
	EIsPlineCCW isPlineCCW(IN AcDbPolyline* pPline);  //interface;

private:
	void calVectorOfVertex(IN const int iIndex); //��ĳ�����㴦��vector3d;
	void getAllVecOfVtx(); //��ȡ���ж����vector;
	void getAllAngles();  //��ȡ���ж��㴦�нǣ���ʱ��н�Ϊ����˳ʱ��Ϊ��;
	void getFirstAng();  //��һ������ļн�Ҫ���⴦��;
	//void getLastAng();  //���һ������ļн�Ҫ���⴦��; ���һ�����ô���;
	EIsPlineCCW isCCW();  //���ݼнǼ����ж��Ƿ���ʱ��;
	bool inputPolyline(IN AcDbPolyline* pPline);	
};


//��ߵ����ڶ˵㴦�������ڻ����м�;
enum ETopY
{
	TOPY_INIT_STATUS,  //��ʼ��״̬;
	TOPY_ON_START,
	TOPY_IN_MIDDLE,
	TOPY_ON_END,
	TOPY_ALL_LINE  //�����߶�����ߵ�;
};

enum ECCW_Pline
{
	ECCW_YES,
	ECCW_NO,
	ECCW_UNKOWN
};


//struct STopY:�������ڶ�����ţ��������ͣ���ߵ�����; 
struct STopY
{
	int m_iIndexVtx;  //�������;
	ETopY m_topYLocation;  //��ߵ��ڻ���ʲôλ��;  
	AcGePoint2d m_pt2d;  //������;
	double m_y;  //������y����ֵ;   
	STopY();
	STopY(const STopY& objSrc);
	~STopY();
};


#define TOL_CCW (0.000001)   //6λС���ľ������ֵ;

//===========================
//class  CJudgeClosedPlineCCW
//�ж϶���������˳��:��ʱ�뻹��˳ʱ��
//�㷨������
//Ѱ����ߵ㼰���ڱߣ�ͨ�������������ж϶�����˳��;  
//===========================
class CJudgePlineCCW
{
private:
	AcDbPolyline* m_pline;
	int m_nNumVerts;  //�����߶������;  

public:
	CJudgePlineCCW(IN AcDbPolyline* pPline);  
	CJudgePlineCCW();
	~CJudgePlineCCW();

public:
	bool inputPline(IN AcDbPolyline* pPline);  
	EIsPlineCCW isPlineCCW();  

private:
	bool prsTopY(OUT struct STopY& sta); //���������ĳ���ߵ���ߵ�״̬; 
	bool prsTopYArcSeg(IN const int iIndexVtx,OUT struct STopY& sta);    
	bool prsTopYLineSeg(IN const int iIndexVtx ,OUT struct STopY& sta);   
	//bool isHigherPtOnArc(AcGeCircArc2d& sGeArc2d);
	EIsPlineCCW whenAllLine(const STopY& sta); //��ߵ���һ��ˮƽ�ߣ��ж������򣨼�˳��ʱ�룩
	EIsPlineCCW whenPtOnStart(const STopY& sta);   //�ж�˳��ʱ��;
	EIsPlineCCW whenPtOnEnd(const STopY& sta);    //�ж�˳��ʱ��; 
	EIsPlineCCW whenPtOnMiddle(const STopY& sta);   //�ж�˳��ʱ��; 
	double calRadian(const STopY& sta);  //����Ƕ�;����Ϊ��λ����Χ[0,2Pi];  

	//bool isHigherPtOn(IN const double startAng,IN const double endAng,IN const double dblBug);
};

#endif //ZHOUHUAGANG_OFSTANDCHAMFER_H