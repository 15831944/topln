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




//½¨Á¢3¸ö½á¹¹£º
//1.´æ´¢ËùÓÐÏß¶ÎµÄ¶Ëµãlist£¨°üÀ¨ÐÂ·¢ÏÖµÄ½»µã;
//2.´æ´¢É¨ÃèÏßÉÏµÄµãset£¨¶þ²æÊ÷£¿£©£»
//3.´æ´¢·¢ÏÖµÄ½»µã;

//¼¸¸öÎÊÌâµã£º
//1.¶ËµãÖØºÏµÄÏß¶ÎÔõÃ´±í´ï;
//2.Ô²»¡ÔõÃ´±íÊ¾³ÉÏß¶Î£¿: »¡¶ÎÓÃÍ¨¹ýÖÐÐÄµÄ´¹ÏßºÍË®Æ½Ïß»®·Ö³ÉËÄ¶Î; Ã¿¶Î»¡ÊÇµ¥µ÷µÄ;
//3.½»µãÊÇÏÂ¶Ëµã£»

//±¾Èí¼þ¹¦ÄÜ£ºÇóN¸öÏß¶ÎµÄ½»µã¼¯;
//²ÉÓÃÉ¨ÃèÏßÏà½»Ëã·¨;
//


//Ö¸Ê¾»¡¶Î·½Î»£º
//»¡¶ÎÓÃÍ¨¹ýÖÐÐÄµÄ´¹ÏßºÍË®Æ½Ïß»®·Ö³ÉËÄ¶Î:×óÉÏ£¬×óÏÂ£¬ÓÒÉÏ£¬ÓÒÏÂ;
enum EPartOfArc
{
	LEFT_TOP,
	LEFT_BOTTOM,
	RIGHT_TOP,
	RIGHT_BOTTOM,
	JUST_ALONE£¬ //±íÊ¾Õâ¸ö»¡±¾Éí¾ÍÊÇµ¥µ÷µÄ;
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



//Ã¶¾Ù£¬Ö¸Ê¾Ïß¶ÎÀàÐÍ
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
ÀàÃû£ºCSegement
ÓÃÍ¾£º±íÊ¾ºÍÉ¨ÃèÏßÏë½»µÄÏß¶Î
±íÊ¾Îª£ºÒ»¸öÏß¶Î£¬¿ÉÄÜÊÇÒ»ÌõÖ±Ïß£¬Ò²¿ÉÄÜÊÇÒ»Ìõ»¡;   
Ò»Ìõ¶àÒ»Ïß·Ö½â³É¶à¸öÖ±Ïß¶ÎºÍ»¡¶Î
»¡¶Î»®·Ö³ÉËÄ¶Î£º»¡¶ÎÓÃÍ¨¹ýÖÐÐÄµÄ´¹ÏßºÍË®Æ½Ïß»®·Ö³ÉËÄ¶Î; Ã¿¶Î»¡ÊÇµ¥µ÷µÄ
*/
class CSegement
{
public:
	CSegement();
	~CSegement(); 

public:
	////Ïß¶ÎµÄ¶Ëµãx×ø±ê	
	//double m_xCoord;
	//double m_yCoord;	

	//±¾Ïß¶Î£¬ÊÇ»¡£¿ÊÇÏß£¿ÊÇ¶àÒåÏß£¿
	ETypeOfArc m_myselfType ; 
	AcDbArc* m_arcPtr;
	AcDbLine* m_LinePtr;	
	EPartOfArc m_whichPart; 

	//Ä¸Ïß¶Î£¬ÊÇ»¡£¿ÊÇÔ²£¿ÊÇ¶àÒåÏß£¿:²»¿ÉÄÜÊÇÏßline;  
	ETypeOfArc m_parentType ;
	AcDbArc* m_arcPtr;
	AcDbCircle* m_circlePtr;
	//AcDbLine* m_LinePtr;
	AcGeCircArc2d* m_circArc2d;
	AcDbPolyline* m_polylinePtr;

	//×æÄ¸Ïß¶Î£¬ÊÇ»¡£¿ÊÇÏß£¿ÊÇ¶àÒåÏß? :ÈôÓÐ£¬Ö»ÄÜÊÇ¶àÒåÏßÁË; 
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
Function:       // º¯ÊýÃû³Æ
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
	bool outputIntersectPoints(OUT CSweeplinePointOpt& sweepLinePnts); 
	bool findAllIntersectPoints();  

private:
	bool prsCurrentIntersectPoints();  //·¢ÏÖµ±Ç°½»µã(¿ÉÄÜÓÐ³¬¹ý2ÌõµÄ»¡¶ÎÏà½»); 
	bool insertIntersectPoints();  

private:  
	//¼ì²âÐÂµÄÊÂ¼þµã; 
	//bool cal();
	bool calNewEventPoints();   

private:
	bool popEventPoint();   
	bool getCurSweepLineSegments();     	 
	bool sortByPointLocation();    
	bool findLeftSegments();   
	bool findRightSegments();   
	bool findFrontSegments();   
	bool findBehindSegments();  
	bool insertNewEventPoints();     

private:
	bool parsePointByLocationType();  

private:
	CIntersectPointOpt m_intsectPointsOpt;    
	CEventPointQueue m_eventPointsOpt;     
	CSweeplinePointOpt m_sweepOpt;   

	vector<SPointAndSegment> m_vEventPoints; //´ÓQÖÐÈ¡³öµÄÊÂ¼þµã;    
	vector<SPointAndSegment> m_vSweepLinePoints; //´ÓÉ¨ÃèÏßÉÏÈ¡³öÀ´µÄ¡°Ïß¶Î¡±;     

	vector<SPointAndSegment> m_vTopPoints; //ÉÏ¶ËµãÊÂ¼þµã¼¯ºÏ; 
	vector<SPointAndSegment> m_vBottomPoints; //ÏÂ¶ËµãÊÂ¼þµã¼¯ºÏ;   
	vector<SPointAndSegment> m_vMiddlePoints; //Ïà½»µã¼¯ºÏ(µãÔÚÏß¶ÎÖÐ¼ä);    
	vector<SPointAndSegment> m_vecPointSegmentsNow; //µ±Ç°ÊÂ¼þµãºÍÉ¨ÃèÏßµã¼¯ºÏ; ÔªËØ×îÉÙÓÐ1¸ö,·ñÔòËµÃ÷³ÌÐòÐ´µÄÓÐÎÊÌâ;  

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
	multiset<SPointAndSegment,eventPointCmp> m_vEventPointsQueue; //ÊÂ¼þµã¼¯ºÏ; 
};




/*
class CSweeplinePointOpt;
¹¦ÄÜ£ºÊÂ¼þ¶ÓÁÐÉÏµÄ²Ù×÷£»
*/
class CSweeplinePointOpt
{
public:
	CSweeplinePointOpt(); 
	~CSweeplinePointOpt();   

private:
	multiset<SPointAndSegment,eventPointCmp> m_vSweepLineSegments; //É¨ÃèÏßÉÏµÄÊÂ¼þµã£¨¼´»¡¶Î£©¼¯ºÏ;  
	vector<SPointAndSegment> m_vNowPointSegs;   
	AcGePoint3d m_point3dNow; //µ±Ç°ÊÂ¼þµã;    

public:   
	bool insertSegment(IN SPointAndSegment& strPntSegPtr);  
	bool findPointSegments(IN AcGePoint3d pt,OUT vector<SPointAndSegment>& vStrPntSeg);    
	bool findPreviousSegment(OUT SPointAndSegment& prePointSegment); //·¢ÏÖ×î×ó±ßsegmengµÄ×ó±ßsegment£» 
	bool findNextSegment(OUT SPointAndSegment& prePointSegment);   //·¢ÏÖ×îÓÒ±ßsegmengµÄÓÒ±ßsegment£» 
	bool findLeftSegmentOf(IN SPointAndSegment& refPointSegment,OUT SPointAndSegment& outPointSegment);   
	bool findRightSegmentOf(IN SPointAndSegment& pointSegment);  //·¢ÏÖ×îÓÒ±ßµÄsegment;     
	bool deleteSegment(IN SPointAndSegment& pointSegment); 
	bool deleteSegments(IN vector<SPointAndSegment>& vSegments);   

public:
	bool ;
};




/*
class CIntersectPointOpt; 
¹¦ÄÜ£º½»µã¼¯ºÏµÄ²Ù×÷£»
*/
class CIntersectPointOpt
{
public:
	CIntersectPointOpt();
	~CIntersectPointOpt();

private:
	bool insertSegment(IN SPointAndSegment* strPntSegPtr);
	bool popPointSegment(IN AcGePoint3d pt,OUT vector<SPointAndSegment>& vStrPntSeg);  

private:
	multiset<SPointAndSegment,eventPointCmp> m_intersectPoints;  //½»µã¼¯ºÏ;
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
class COptOnPointSegmentGroup; 
¹¦ÄÜ£º;
×¢Òâ£º

*/
class COptOnPointSegmentGroup
{
public:
	COptOnPointsGroup();  
	~COptOnPointsGroup();    
public:
	bool sortByAngle(IN OUT vector<SPointAndSegment>& vPoints); //°´ºÍÉ¨ÃèÏßÀë¿ªºóÏà½»Ë³ÐòÅÅÐò;
	bool getLeftSegment(OUT vector<SPointAndSegment>& vPoints);  
	bool getRightSegment(OUT vector<SPointAndSegment>& vPoints);  
private:
	vector<SPointAndSegment> m_vPointsGroup;   
};



/*
class ; 
¹¦ÄÜ£º;
×¢Òâ£º
*/


#endif  //ZHOUHUAGANG_20190822_segments_intersections