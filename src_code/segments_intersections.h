/*************************************************************************
Copyright (C), 2017-12-1, XuMiJieZi.
FileName: pointMap.h
Author: 
Version:
Date: //zhg 1.0
Description: // 1.��ѡ�е�ʵ�弯�Ͻ������������ѯ;
             // 2.ʹ��map�ṹ; �Ե������x��yΪ��ѯkeyֵ; x��mapǶ��y��map��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
eg�� Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20190822_segments_intersections
#define  ZHOUHUAGANG_20190822_segments_intersections

#include <map>   
#include <algorithm>
#include <vector>
#include <list>
#include <set>
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


using namespace  std;




//����3���ṹ��
//1.�洢�����߶εĶ˵�list�������·��ֵĽ���;
//2.�洢ɨ�����ϵĵ�set��������������
//3.�洢���ֵĽ���;

//��������㣺
//1.�˵��غϵ��߶���ô���;
//2.Բ����ô��ʾ���߶Σ�: ������ͨ�����ĵĴ��ߺ�ˮƽ�߻��ֳ��Ķ�; ÿ�λ��ǵ�����;
//3.�������¶˵㣻

//��������ܣ���N���߶εĽ��㼯;
//����ɨ�����ཻ�㷨;
//


//ָʾ���η�λ��
//������ͨ�����ĵĴ��ߺ�ˮƽ�߻��ֳ��Ķ�:���ϣ����£����ϣ�����;
enum EPartOfArc
{
	LEFT_TOP,
	LEFT_BOTTOM,
	RIGHT_TOP,
	RIGHT_BOTTOM,
	JUST_ALONE�� //��ʾ�����������ǵ�����;
	NONE_NONE
};


//ָʾĳ���������߶ε��϶˵㻹���¶˵�,��ƽ���߶εĻ�������˵㻹���Ҷ˵�;
enum ELocationTypeOfPoint
{
	TOP_POINT,
	BOT_POINT,
	LEFT_POINT,
	DOWN_POINT,
	MIDDLE_POINT,
	NONE_INDICTATION  //����ָʾ;
};



//ö�٣�ָʾ�߶�����
enum ETypeOfArc
{
	Line_Type,
	Arc_Type,
	Circle_Type,
	Polyline_Type,
	AcGeCircArc2d_Type,
	None_Type //problem type;
};

/*
������CSegement
��;����ʾ��ɨ�����뽻���߶�
��ʾΪ��һ���߶Σ�������һ��ֱ�ߣ�Ҳ������һ����;   
һ����һ�߷ֽ�ɶ��ֱ�߶κͻ���
���λ��ֳ��ĶΣ�������ͨ�����ĵĴ��ߺ�ˮƽ�߻��ֳ��Ķ�; ÿ�λ��ǵ�����
*/
class CSegement
{
public:
	CSegement();
	~CSegement(); 

public:
	////�߶εĶ˵�x����	
	//double m_xCoord;
	//double m_yCoord;	

	//���߶Σ��ǻ������ߣ��Ƕ����ߣ�
	ETypeOfArc m_myselfType ; 
	AcDbArc* m_arcPtr;
	AcDbLine* m_LinePtr;	
	EPartOfArc m_whichPart; 

	//ĸ�߶Σ��ǻ�����Բ���Ƕ����ߣ�:����������line;  
	ETypeOfArc m_parentType ;
	AcDbArc* m_arcPtr;
	AcDbCircle* m_circlePtr;
	//AcDbLine* m_LinePtr;
	AcGeCircArc2d* m_circArc2d;
	AcDbPolyline* m_polylinePtr;

	//��ĸ�߶Σ��ǻ������ߣ��Ƕ�����? :���У�ֻ���Ƕ�������; 
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
struct SPointAndSegment
���ܣ�����һ�����һ�����ε���ϣ�
      �����¼��㡢ɨ�����ϵĻ��εĽṹ�ı��;
	  �����¼���Ľṹ��
	  ����洢�ṹ;
	  �¼���洢���¼�Q��ɨ�����뽻�߶νṹT�У�
*/
struct SPointAndSegment
{
	AcGePoint3d m_point;
	ELocationTypeOfPoint m_ePointLocation;  
	CSegement* m_segment;  
};




///*
//class CIntersectPoint;
//���彻��ṹ��
//����Ϊ�����Ľ��㣬��ɨ������з��ֵĽ���;
//*/
//class CIntersectPoint
//{
//public:
//	CIntersectPoint();
//	~CIntersectPoint();
//
//private:
//	double m_x;
//	double m_y;
//	AcGePoint3d m_acgePoint;
//
//	CSegement m_segment1;  
//	ELocationTypeOfPoint m_eLocationInSeg1;  
//	CSegement m_segment2;
//	ELocationTypeOfPoint m_eLocationInSeg2;   
//};





/*
������ȡ�߶�CBreakArcToFourSegment;
�����ηֳ��Ĳ��֣���Բ�Ļ���ֱ�ߺ�ˮƽ�ߣ���Բ���и���Ĳ��֣�
����ÿ���ֶ��ǵ����Ļ�;
*/
class CBreakArcToFourPart
{
public:
	CBreakArcToFourPart(); 
	~CBreakArcToFourPart(); 
	CBreakArcToFourPart(AcGeCircArc2d* pGeArc2d);

private:
	AcGeCircArc2d* m_geArc2dToBreaked;   //��Ҫ���ƵĻ���;   
	AcGeCircArc2d* m_geArc2dLeftTop;
	AcGeCircArc2d* m_geArc2dLeftBot;  
	AcGeCircArc2d* m_geArc2dRightTop;  
	AcGeCircArc2d* m_geArc2dRightBot;   
	vector<AcGeCircArc2d*> m_arcs;    

private:
	bool breakArcToFourParts(); //Ҳ������С��4������;  

public:
	bool inputArcSegToBreaked(AcGeCircArc2d* pGeArc2d);  //������Ҫ����Ļ���;    
	bool outputWhatPartAng(OUT AcGeCircArc2d* geArcPartOne,IN EPartOfArc whatPart); //�����һ���޻��Σ�����еĻ���
};



/*
class CBreakAcGeCircArcToTwoPart
���ܣ�������Ļ���������ĽǶȣ����Ƴ�2�λ�;
���룺���Σ��Ƕ�;
��������ƺ��2�λ�;Ҳ���ܽǶȲ��ڻ����ϣ�������;
*/
class CBreakAcGeCircArcToTwoPart
{
private:
	AcGeCircArc2d* m_inputArc;
	double m_inputRadianToBreak;  
	AcGeCircArc2d* m_arcResult1;   
	AcGeCircArc2d* m_arcResult2; 	
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


/*
class CQuadrantAnglePair
���ܣ����ݻ��β�λ���ж����ޣ��ó�������С�Ƕȣ����Ƕ�;
*/
class CQuadrantAnglePair
{
public:
	CQuadrantAnglePair();
	~CQuadrantAnglePair();
public:
	bool getQuadrantAnglePair(IN EPartOfArc whichPart,OUT double& startAngle,OUT double& endAngle);
};




/*
class CAcGeCircArcToAcDbArc
���ܣ���AcGeCircArc2d��AcDbArc2dת��;
*/
class CAcGeCircArcToAcDbArc
{
public:
	CAcGeCircArcToAcDbArc();
	~CAcGeCircArcToAcDbArc();
public:
	bool transToAcDbArc();
private:
	AcGeCircArc2d* m_geArcPtr;  
	AcDbArc* m_dbArcPtr;
};



/*
���ܣ��Ƚ��������λ�ô�С;
��С���������ֵequalPoint�ڣ��������;�����߲űȽϴ�С;  
*/



/*
����Ŀ��multiset��Ԫ��֮���С�Ƚϣ��ṹ;
��������Ƚ��¼��㼯���еĵ㣬��yֵ��С��������   
*/
struct multisetCmpEventPoint
{
	bool operator()(const SPointAndSegment& ps1,const SPointAndSegment& ps2)const  
	{
		double x1 = ps1.m_point.x; 
		double y1 = ps1.m_point.y;  
		double x2 = ps2.m_point.x;  
		double y2 = ps2.m_point.y;  

		//���2������벻����equalPoint()���ж����;   
		if(ps1.m_point.distanceTo(ps2) <= AcGeTol::equalPoint())   
		{
			return false;  //�����ȣ��²���ĵ�������;
		}
		else
		{
			return (y1 > y2) || (y1 == y2 && x1 < x2);
		}
	}
}


/*************************************************
class CParseIntersectPoints
Function:       // ��������
Description:    // �������ܡ����ܵȵ�����
Calls:          // �����������õĺ����嵥
Input:          // �������˵��������ÿ����������
                  // �á�ȡֵ˵�����������ϵ��
Output:         // �����������˵����
Return:         // ��������ֵ��˵��
Others:         // ����˵��
*************************************************/
class CParseIntersectPoints
{
public:
	CParseIntersectPoints();
	~CParseIntersectPoints();

public:
	bool findIntersectPoints(); 

private: 
	bool initSegmentsAll();
	bool ;

private:
	vector<vector<SPointAndSegment>> m_vecIntersectPoints; //��Ž���;
	multiset<SPointAndSegment,multisetCmpEventPoint> m_vEventPointsQueue; //�¼���
	multiset<SPointAndSegment,multisetCmpEventPoint> m_vSweepLinePointsQueue;  //ɨ�����뽻�Ļ���; 
	;
};


/*
class CEventPointQueue;
*/


#endif  //ZHOUHUAGANG_20190822_segments_intersections