/*************************************************************************
Copyright (C), 2017-12-1, XuMiJieZi.
FileName: pointMap.h
Author: 
Version:
Date: //zhg 1.0
Description: // 1.¶ÔÑ¡ÖÐµÄÊµÌå¼¯ºÏ½¨Á¢¶¥µã±í£¬¹©²éÑ¯;
             // 2.Ê¹ÓÃmap½á¹¹; ÒÔµã×ø±êµÄx¡¢yÎª²éÑ¯keyÖµ; xµÄmapÇ¶Ì×yµÄmap£»
Function List: // Ö÷Òªº¯ÊýÁÐ±í£¬Ã¿Ìõ¼ÇÂ¼Ó¦°üÀ¨º¯ÊýÃû¼°¹¦ÄÜ¼òÒªËµÃ÷
1. ....
History: // ÐÞ¸ÄÀúÊ·¼ÇÂ¼ÁÐ±í£¬Ã¿ÌõÐÞ¸Ä¼ÇÂ¼Ó¦°üÀ¨ÐÞ¸ÄÈÕÆÚ¡¢ÐÞ¸Ä
		// Õß¼°ÐÞ¸ÄÄÚÈÝ¼òÊö
eg£º Date:	Author: 	Modification:
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




//扫描线算法求N条弧段的所有交点;

//Ö¸Ê¾»¡¶Î·½Î»£º
//»¡¶ÎÓÃÍ¨¹ýÖÐÐÄµÄ´¹ÏßºÍË®Æ½Ïß»®·Ö³ÉËÄ¶Î:×óÉÏ£¬×óÏÂ£¬ÓÒÉÏ£¬ÓÒÏÂ;
enum EPartOfArc
{
	LEFT_TOP,
	LEFT_BOTTOM,
	RIGHT_TOP,
	RIGHT_BOTTOM,
	JUST_ALONE,  //±íÊ¾Õâ¸ö»¡±¾Éí¾ÍÊÇµ¥µ÷µÄ;
	NONE_NONE
};


//Ö¸Ê¾Ä³¸ö¶¥µãÊÇÏß¶ÎµÄÉÏ¶Ëµã»¹ÊÇÏÂ¶Ëµã,ÊÇÆ½ÐÐÏß¶ÎµÄ»°£¬ÊÇ×ó¶Ëµã»¹ÊÇÓÒ¶Ëµã;
enum ELocationTypeOfPoint
{
	TOP_POINT,
	BOT_POINT,
	LEFT_POINT,
	RIGHT_POINT,
	MIDDLE_POINT,
	NONE_INDICTATION  //´íÎóÖ¸Ê¾;
};



//弧段类型;
enum ETypeOfArc
{
	Line_Type,
	Arc_Type,
	Circle_Type,
	Polyline_Type,
	AcGeCircArc2d_Type,
	None_Type //problem type;
};


//一条弧段的信息;
struct Segment
{
	ETypeOfArc m_myselfType ; 
	AcDbArc* m_arcPtr;
	AcDbLine* m_LinePtr;	
	EPartOfArc m_whichPart; 
};


//弧段父辈信息,如果父辈存在的话;
struct SegmentFather
{
	//父母信息(可能存在）；  
	ETypeOfArc m_parentType ;k
	AcDbArc* m_arcPtr;
	AcDbCircle* m_circlePtr;
	//AcDbLine* m_LinePtr;
	AcGeCircArc2d* m_circArc2d;
	AcDbPolyline* m_polylinePtr;
	
};



//弧段祖父的信息，如果存在祖父辈的话：
struct SegmentGrandfather
{
	//祖父母信息（可能存在); 
	ETypeOfArc m_grandmaType;
	//AcDbArc* m_arcPtr;
	//AcDbLine* m_LinePtr;
	AcDbPolyline* m_polylinePtr;  
	int index; //多义线中的第几段子弧;
};

/*
class CSegement
功能：记录一条弧段的信息;
*/
struct CSegement
{
	//segment
	Segment* m_itself；
	//father
	SegmentFather* m_father;
	SegmentGrandfather* m_granfather;s

public:
	bool initCSegement(IN AcDbLine* dbLinePtr);
	bool initCSegement(IN AcDbPolyline* polylinePtr,IN int indexVertex,IN int arcPartNumber); 
	bool initCSegement(IN AcDbCircle* circlePtr,IN int arcPartNumber); 
	bool initCSegement(IN AcDbArc* arcPtr, IN int arcPartNumber);
};




/*
»¡¶ÎÌáÈ¡Ïß¶ÎCBreakArcToFourSegment;
½«»¡¶Î·Ö³ÉËÄ²¿·Ö£ºÒÔÔ²ÐÄ»­´¹Ö±ÏßºÍË®Æ½Ïß£¬½«Ô²»¡ÇÐ¸î³ÉËÄ²¿·Ö£»
ÕâÑùÃ¿²¿·Ö¶¼ÊÇµ¥µ÷µÄ»¡;
*/
class CBreakArcToFourPart
{
public:
	CBreakArcToFourPart(); 
	~CBreakArcToFourPart(); 
	CBreakArcToFourPart(AcGeCircArc2d* pGeArc2d);

private:
	AcGeCircArc2d* m_geArc2dToBreaked;   //ÐèÒª´òÆÆµÄ»¡¶Î;   
	AcGeCircArc2d* m_geArc2dLeftTop;
	AcGeCircArc2d* m_geArc2dLeftBot;  
	AcGeCircArc2d* m_geArc2dRightTop;  
	AcGeCircArc2d* m_geArc2dRightBot;   
	vector<AcGeCircArc2d*> m_arcs;    

private:
	bool breakArcToFourParts(); //Ò²Ðí´òËéºóÐ¡ÓÚ4¸ö²¿·Ö;  

public:
	bool inputArcSegToBreaked(AcGeCircArc2d* pGeArc2d);  //ÊäÈëÐèÒª´òËéµÄ»¡¶Î;    
	bool outputWhatPartAng(OUT AcGeCircArc2d* geArcPartOne,IN EPartOfArc whatPart); //Êä³öµÚÒ»ÏóÏÞ»¡¶Î£¬Èç¹ûÓÐµÄ»°£»
};



/*
class CBreakAcGeCircArcToTwoPart
¹¦ÄÜ£º½«ÊäÈëµÄ»¡£¬°´ÊäÈëµÄ½Ç¶È£¬´òÆÆ³É2¶Î»¡;
ÊäÈë£º»¡¶Î£¬½Ç¶È;
Êä³ö£º´òÆÆºóµÄ2¶Î»¡;Ò²¿ÉÄÜ½Ç¶È²»ÔÚ»¡¶ÎÉÏ£¬²»´òÆÆ;
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
¹¦ÄÜ£º¸ù¾Ý»¡¶Î²¿Î»£¬ÅÐ¶ÏÏóÏÞ£¬µÃ³öÏóÏÞ×îÐ¡½Ç¶È£¬×î´ó½Ç¶È;
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
¹¦ÄÜ£º´ÓAcGeCircArc2dµ½AcDbArc2d×ª»»;
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
¹¦ÄÜ£º±È½ÏÁ½¸öµãµÄÎ»ÖÃ´óÐ¡;
´óÐ¡¹æÔò£ºÔÚÎó²îÖµequalPointÄÚ£¬ÔòÅÐÏàµÈ;³¬³öÕß²Å±È½Ï´óÐ¡;  
*/



/*
±¾ÏîÄ¿µÄmultisetµÄÔªËØÖ®¼ä´óÐ¡±È½Ï£¬½á¹¹;
Õâ¸öÓÃÀ´±È½ÏÊÂ¼þµã¼¯ºÏÖÐµÄµã£¬°´yÖµ´ÓÐ¡µ½´óÅÅÐò£»   
*/
struct eventPointCmp
{
	bool operator()(const SPointAndSegment& ps1,const SPointAndSegment& ps2)const  
	{
		double x1 = ps1.m_point.x; 
		double y1 = ps1.m_point.y;  
		double x2 = ps2.m_point.x;  
		double y2 = ps2.m_point.y;  

		//Èç¹û2¸öµã¾àÀë²»´óÓÚequalPoint()ÔòÅÐ¶¨ÏàµÈ;   
		if(ps1.m_point.distanceTo(ps2) <= AcGeTol::equalPoint())   
		{
			return false;  //Èç¹ûÏàµÈ£¬ÐÂ²åÈëµÄµãÍùºóÅÅ;
		}
		else
		{
			return (y1 > y2) || (y1 == y2 && x1 < x2);
		}
	}
}



/*
struct SPointAndSegment
¹¦ÄÜ£º¶¨ÒåÒ»¸öµãºÍÒ»¸ö»¡¶ÎµÄ×éºÏ£»
      ÓÃÓÚÊÂ¼þµã¡¢É¨ÃèÏßÉÏµÄ»¡¶ÎµÄ½á¹¹µÄ±í´ï;
	  ¶¨ÒåÊÂ¼þµãµÄ½á¹¹£»
	  ½»µã´æ´¢½á¹¹;
	  ÊÂ¼þµã´æ´¢ÓÚÊÂ¼þQ¼°É¨ÃèÏßÏë½»Ïß¶Î½á¹¹TÖÐ£»
*/
struct SPointAndSegment
{
	AcGePoint3d m_point;
	ELocationTypeOfPoint m_ePointLocation;  
	CSegement* m_segment;  
};




///*
//class CIntersectPoint;
//¶¨Òå½»µã½á¹¹£»
//½»µãÎªÕæÕýµÄ½»µã£¬ÔÚÉ¨Ãè¹ý³ÌÖÐ·¢ÏÖµÄ½»µã;   
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




/*************************************************
class CParseIntersectPoints
Function:       // 求交点;
Description:    // º¯Êý¹¦ÄÜ¡¢ÐÔÄÜµÈµÄÃèÊö
Calls:          // ±»±¾º¯Êýµ÷ÓÃµÄº¯ÊýÇåµ¥
Input:          // ÊäÈë²ÎÊýËµÃ÷£¬°üÀ¨Ã¿¸ö²ÎÊýµÄ×÷
                  // ÓÃ¡¢È¡ÖµËµÃ÷¼°²ÎÊý¼ä¹ØÏµ¡£
Output:         // ¶ÔÊä³ö²ÎÊýµÄËµÃ÷¡£
Return:         // º¯Êý·µ»ØÖµµÄËµÃ÷
Others:         // ÆäËüËµÃ÷
*************************************************/
class CParseIntersectPoints
{
public:
	CParseIntersectPoints();  
	~CParseIntersectPoints();   


public:
	bool outputIntersectPoints(OUT vector<SPointAndSegment>& vAllIntersectPoints); 
	bool prsAllIntersectPoints();  

private:
	bool chkCurrentIntersectPoints();  //检查当前点是否相交点;     
	bool insertIntersectPoints();  

private:
	bool popEventPoint();   
	bool getCurSweepLineSegments();     
	bool sortByPointLocation();    	  

private:
	CSegmentsOpt m_intsectPointsOpt;    
	CEventPointQueue m_eventPointsOpt;     
	CSweeplinePointOpt m_sweepOpt;   

	vector<SPointAndSegment> m_vEventPoints; //所有当前事件点;  
	vector<SPointAndSegment> m_vSweepLinePoints; //扫描线上的当前弧段（也是事件点);     

	vector<SPointAndSegment> m_vTopPoints; //上端点事件; 
	vector<SPointAndSegment> m_vBottomPoints; //下端点事件;   
	vector<SPointAndSegment> m_vMiddlePoints; //中端点事件;    
	vector<SPointAndSegment> m_vecPointSegmentsNow; //所有事件点;    

	AcGePoint3d m_curPoint; //当前坐标点;    
	//SPointAndSegment m_curPointAndSegment; //µ±Ç°»¡¶Î£ºÕâ¸ö²ÎÊýÃ»ÒâÒå£¬µ±Ç°»¡¶Î¿ÉÄÜ²»Ö¹Ò»Ìõ£»
};


/*
class CEventPointOpt;
¹¦ÄÜ£ºÊÂ¼þ¶ÓÁÐÉÏµÄ²Ù×÷£»
ÊÇ²»ÊÇÍ¬Ñù¿ÉÒÔÓÃÓÚÉ¨ÃèÏßÏà½»»¡¶ÎµÄ²Ù×÷£¿£¿£¿  
*/
class CEventPointQueue
{
public:
	CEventPointOpt();   
	~CEventPointOpt();  

private:
	bool initSegmentsAll();  //³õÊ¼»¯ËùÓÐ»¡¶Î;»ñÈ¡¶Ëµã,²åÈëÊÂ¼þ¶ÓÁÐ;       
	bool insertEventPoint(IN const SPointAndSegment& eventPoint);   
	bool popOneEventPoint(OUT vector<SPointAndSegment>& vecEventPoints);    

private:
	multiset<SPointAndSegment,eventPointCmp> m_vEventPointsQueue; //事件点集合; 
};  




/*
class CSweeplinePointOpt;
功能：扫描线上的各种操作;包括:1-插入新的弧段，2-删除弧段，
      3-查找当前坐标点相关新的事件点（新的交点）;
*/
class CSweeplinePointOpt
{
public:
	CSweeplinePointOpt(); 
	~CSweeplinePointOpt();    

public:   
	bool insertSegment(IN SPointAndSegment& strPntSegPtr);  
	bool deleteSegment(IN SPointAndSegment& pointSegment); 
	bool deleteSegments(IN vector<SPointAndSegment>& vSegments); 
	bool prsIntersectPoints();  

private:
	multiset<SPointAndSegment,eventPointCmp> m_vSweepLineSegments; //É¨ÃèÏßÉÏµÄÊÂ¼þµã£¨¼´»¡¶Î£©¼¯ºÏ;  
	vector<SPointAndSegment> m_vNowPointSegs;   
	AcGePoint3d m_point3dNow; //µ±Ç°ÊÂ¼þµã;    

private:
	bool findLeftSegments();   
	bool findRightSegments();   
	bool findFrontSegments();   
	bool findBehindSegments();  

private:
	//µ±Ç°ÊÂ¼þµãµÄÇ°Ò»¸öÊÂ¼þµã»¡¶Î£» ºÍµ±Ç°ÊÂ¼þµãµÄÏÂÒ»¸öÊÂ¼þµã»¡¶Î£»  --ÎªÁËÇó½»µã£¨ÐÂÊÂ¼þ£©£»
	vector<SPointAndSegment> m_frontSegment;    //µ±Ç°»¡¶ÎµÄÇ°Ò»¸ö»¡¶ÎÈºµÄ×îÓÒ±ß»¡¶Î¿ÉÄÜÓÐ¶à¸ö;
	//µ±Ç°»¡¶ÎµÄÏÂÒ»¸ö»¡¶ÎÈºµÄ×î×ó±ß»¡¶Î¿ÉÄÜÓÐÒ»¸ö£¨²»Æ½ÐÐµÄ»¡¶Î£©£¬Ò²¿ÉÄÜÓÐ¶à¸ö£¨Æ½ÐÐ»¡¶Î£©;
	vector<SPointAndSegment>  m_vecBehindSegments;   
	vector<SPointAndSegment>  m_vecleftSegments;   //µ±Ç°»¡¶Î×î×ó±ß±ß»¡¶Î£¬¿ÉÄÜ²»Ö¹Ò»¸ö,Èç¹û²»Ö¹Ò»¸ö£¬Ôò¶¼ÊÇÆ½ÐÐµÄ;
	vector<SPointAndSegment > m_vecRightSegments; //µ±Ç°»¡¶Î×îÓÒ±ß»¡¶Î£¬¿ÉÄÜ²»Ö¹Ò»¸ö,Èç¹û²»Ö¹Ò»¸ö£¬Ôò¶¼ÊÇÆ½ÐÐµÄ;
	vector<SPointAndSegment> m_vecNewEventPoints;  //¼ÆËã³öÀ´µÄÐÂµÄÊÂ¼þµã;    

	int m_topPointsNum; //ÉÏ¶ËµãÊÂ¼þµãÊýÁ¿;    
	int m_botPointsNum; //ÏÂ¶ËµãÊÂ¼þµãÊýÁ¿;   
	int m_curMiddlePointsNum; //Ïà½»µãÊýÁ¿,Ò²ÐíÖ»ÓÐÒ»Ìõ»¡¶Î£¬ÓÐ2Ìõ¼°ÒÔÉÏ»¡¶ÎÔòËµÃ÷ÊÇ½»µã;    

	AcGePoint3d m_curPoint; //µ±Ç°×ø±êµã;    
};




/*
class CSegmentsOpt; 
功能：
*/
class CTwoSegmentsOpt
{
public:
	CTwoSegmentsOpt();
	CTwoSegmentsOpt(IN SPointAndSegment s1,IN SPointAndSegment s2);
	~CTwoSegmentsOpt();

public:
	bool prsNewIntersectPoints(OUT vector<SPointAndSegment>& vPoints); 

private:
	;

private:
	SPointAndSegment m_seg1;  
	SPointAndSegment m_seg2;  
};




/*
class CParseIntersectPointByTwoSegment; 
¹¦ÄÜ£ºÇóÁ©»¡¶ÎµÄ½»µã£¬¼´ÐÂµÄÊÂ¼þµã(½»µãÀàÐÍ);
×¢Òâ£º
1.Èç¹û2¸ösegmentsÒÑ¾­ÓÐ½»µã£¬ÔòÐÂµÄ½»µã²»ÄÜÐ¡ÓÚÕâ¸ö½»µã;
2.Èç¹û2¸ösegmentsÃ»ÓÐ½»µã£¬ÔòÇóµÄ½»µãÈçºÎ±£Ö¤²»ÊÇ¡°¾É½»µã¡±£¿
  ½â´ð£ºÈç¹ûÊÇ¾É½»µã£¬ÇÒÒÑ¾­¾­¹ýÊÂ¼þ´¦Àí,ÔÚÒÑ¾­·¢ÏÖµÄ½»µã¼¯ºÏÀïÓ¦¸Ã´æÔÚ;  
        Èç¹ûÊÇ¾É½»µã£¬»¹ÔÚÊÂ¼þ¼¯ºÏÀï£¬ÊÂ¼þ¼¯ºÏ²»»áÖØ¸´²åÈë;
3.ÕâÀïÖ»¹Ü¼ÆËã³ö½»µã?
*/
class CIntersectTwoSegments
{
public:
	CIntersectTwoSegments();   
	CIntersectTwoSegments(IN const SPointAndSegment& segment1,IN const SPointAndSegment& segment2);
	~CIntersectTwoSegments();    

public:
	bool input(IN const SPointAndSegment& segment1,IN const SPointAndSegment& segment2);   
	bool output(OUT vector<SPointAndSegment>& vIntersectPoints);  
private:
	SPointAndSegment m_firstSegment;    
	SPointAndSegment m_secondSegment;     
	//SPointAndSegment ;    
	vector<SPointAndSegment> m_vecIntersectPointsResult;   
};



/*
class COptOnSegmentsGroup; 
功能：对多个事件点（有共同交点）进行操作；
      操作包括:找出最左边弧段、最右边弧段;
*/
class COptOnSegmentsGroup  
{
public:
	COptOnSegmentsGroup();  
	~COptOnSegmentsGroup();    
public:
	bool sortByAngle(IN OUT vector<SPointAndSegment>& vPoints); //°´ºÍÉ¨ÃèÏßÀë¿ªºóÏà½»Ë³ÐòÅÅÐò;
	bool getLeftSegment(OUT vector<SPointAndSegment>& vPoints);  
	bool getRightSegment(OUT vector<SPointAndSegment>& vPoints);  
private:
	vector<SPointAndSegment> m_vPointsGroup;   
};



/*
class CPrsTangencyOfSegment; 
给定弧段及其上一个点，计算其切线角度;
*/
class CPrsTangencyOfSegment
{
public:
	CPrsTangencyOfSegment();  	
	~CPrsTangencyOfSegment();   

public:
	virtual bool init(IN const SPointAndSegment* pPntSegment) = 0;      
	virtual double calTopPointTangency() = 0;    
	virtual double calBotPointTangency() = 0;   
	virtual double calMidPointTangency() = 0;	   

//private:	 
};


/*
class CParsTangencyOnArc; 
给定弧段及其上一个点，计算其切线角度;
Top point:是指Y值更大;或者在Y值相等情况下，左边端点为Top point;
*/
class CParsTangencyOnArc
{
public:
	CParsTangencyOnArc();  
	CParsTangencyOnArc(IN AcDbArc* pArc);
	CParsTangencyOnArc(IN AcGePoint3d midPoint);
	CParsTangencyOnArc(IN AcDbArc* pArc,IN AcGePoint3d midPoint);  
	~CParsTangencyOnArc();  

public:
	bool init(IN AcDbArc* pArc);
	bool init(IN AcGePoint3d midPoint);
	bool init(IN AcDbArc* pArc,IN AcGePoint3d midPoint);  
	void calTopAndBotPointTangency();   
	//bool calBotPointTangency();   
	void calMidToTopAndBotTangency();  
	//bool calMidToBotTangency();   
	double rtnTopPointTangency();   
	double rtnBotPointTangency();  
	double rtnMidToTopTangency();  
	double rtnMidToBotTangency(); 

private:
	COptOnArc* m_pArcOpt;  
	AcGePoint3d m_midPoint;    
	double m_topPointTangency;   
	double m_botPointTangency;   
	double m_midToBotTangency;  
	double m_midToTopTangency;   
}




/*
class COptOnArc; 
功能：针对arc类型实体的一些计算；
*/
class COptOnArc    
{
public:
	COptOnArc();  
	COptOnArc(IN const AcDbArc* pArc);
	COptOnArc(IN const AcGePoint3d midPoint);   
	COptOnArc(IN const AcDbArc* pArc,IN AcGePoint3d midPoint);   
	~COptOnArc();   

public:  
	 bool init(IN const AcDbArc* pArc,IN AcGePoint3d midPoint);  
	 bool init(IN const AcDbArc* pArc); 
	 bool init(IN AcGePoint3d midPoint); //弧段上的点： 
	 double calStartPointTangency();   
	 double calEndPointTangency();   
	 bool calMidPointTangency();   

	 double rtnTangencyFromMidToStartPoint();   
	 double rtnTangencyFromMidToEndPoint();     
	 double rtnTangencyFromStartPoint();     
	 double rtnTangencyFromEndPoint();	  
public:
	bool isStartPointEqualToEndPoint();   
	bool isStartPointLowerThanEndPointInYcoord();   
private:
	 //void prsTopAndBotPoint();   
	 bool isTheMidPointEqualStartPoint();     
	 bool isTheMidPointEqualEndPoint();    
	 bool isTheMidPointOnArc();  	
	 void getBaseInfoOfArc();   


private:
	void calStartPoint();
	void calEndPoint();
	bool isStartPointEqualToEndPoint();    

private:
	AcDbArc* m_pArc;   	
	double r;
	AcGePoint3d m_centerPoint;  
	double m_startAngle;   
	double m_endAngle;   

private:
	AcGePoint3d m_midPoint; 
	AcGePoint3d m_startPoint;   
	AcGePoint3d m_endPoint;
	double m_startPointTangency;   
	double m_endPointTangency;
	double m_tanFromMidToStartPoint;   
	double m_tanFromMidToEndPoint;	
};




/*
class COptOnLine; 
功能：针对AcDbLine体的一些计算；
*/
class COptOnLine    
{
public:
	COptOnLine();  
	COptOnLine(IN const AcDbLine* pLine);
	//COptOnLine(IN const AcGePoint3d midPoint);   
	//COptOnLine(IN const AcDbLine* pLine,IN AcGePoint3d midPoint);   
	~COptOnLine();   

public:  
	//bool init(IN const AcDbLine* pLine,IN AcGePoint3d midPoint);    
	bool init(IN const AcDbLine* pLine); 
	//bool init(IN AcGePoint3d midPoint); //弧段上的点： 
	double calStartPointTangency();   
	double calEndPointTangency();   
	//void calMidPointTangency();   

	//double rtnTangencyFromMidToStartPoint();   
	//double rtnTangencyFromMidToEndPoint();     
	double rtnTangencyFromStartPoint();     
	double rtnTangencyFromEndPoint();	  
public:
	bool isStartPointEqualToEndPoint();   
	bool isStartPointLowerThanEndPointInYcoord();     
private:
	//void prsTopAndBotPoint();   
	//bool isTheMidPointEqualStartPoint();     
	//bool isTheMidPointEqualEndPoint();    
	//bool isTheMidPointOnArc();  	
	void getBaseInfoOfArc();   


private:
	void calStartPoint();
	void calEndPoint();
	bool isStartPointEqualToEndPoint();    

private:
	AcDbLine* m_pLine;   			
	//double m_startAngle;   
	//double m_endAngle;   

private:
	//AcGePoint3d m_midPoint; 
	AcGePoint3d m_startPoint;   
	AcGePoint3d m_endPoint;
	double m_startPointTangency;   
	double m_endPointTangency;
	//double m_tanFromMidToStartPoint;    
	//double m_tanFromMidToEndPoint;	
};




/*
class ConvertArcToArc
功能：AcGeCircArc3d和AcDbArc互相转换；
*/
class ConvertArcToArc
{
public:
	ConvertArcToArc();
	~ConvertArcToArc();

public:
	void convertArc2Arc( AcGeCircArc3d*pGeArc, AcDbArc*&pDbArc );
	void convertArc2Arc( AcDbArc*pDbArc, AcGeCircArc3d*&pGeArc);
};

#endif  //ZHOUHUAGANG_20190822_segments_intersections