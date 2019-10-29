/*************************************************************************
Copyright (C), 2017-12-1, XuMiJieZi.
FileName: pointMap.h
Author: 
Version:
Date: //zhg 1.0
Description: // 1.对选中的实体集合建立顶点表，供查询;
             // 2.使用map结构; 以点坐标的x、y为查询key值; x的map嵌套y的map；
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改
		// 者及修改内容简述
eg： Date:	Author: 	Modification:
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




//建立3个结构：
//1.存储所有线段的端点list（包括新发现的交点;
//2.存储扫描线上的点set（二叉树？）；
//3.存储发现的交点;

//几个问题点：
//1.端点重合的线段怎么表达;
//2.圆弧怎么表示成线段？: 弧段用通过中心的垂线和水平线划分成四段; 每段弧是单调的;
//3.交点是下端点；

//本软件功能：求N个线段的交点集;
//采用扫描线相交算法;
//


//指示弧段方位：
//弧段用通过中心的垂线和水平线划分成四段:左上，左下，右上，右下;
enum EPartOfArc
{
	LEFT_TOP,
	LEFT_BOTTOM,
	RIGHT_TOP,
	RIGHT_BOTTOM,
	JUST_ALONE， //表示这个弧本身就是单调的;
	NONE_NONE
};


//指示某个顶点是线段的上端点还是下端点,是平行线段的话，是左端点还是右端点;
enum ELocationTypeOfPoint
{
	TOP_POINT,
	BOT_POINT,
	LEFT_POINT,
	RIGHT_POINT,
	MIDDLE_POINT,
	NONE_INDICTATION  //错误指示;
};



//枚举，指示线段类型
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
类名：CSegement
用途：表示和扫描线想交的线段
表示为：一个线段，可能是一条直线，也可能是一条弧;   
一条多一线分解成多个直线段和弧段
弧段划分成四段：弧段用通过中心的垂线和水平线划分成四段; 每段弧是单调的
*/
class CSegement
{
public:
	CSegement();
	~CSegement(); 

public:
	////线段的端点x坐标	
	//double m_xCoord;
	//double m_yCoord;	

	//本线段，是弧？是线？是多义线？
	ETypeOfArc m_myselfType ; 
	AcDbArc* m_arcPtr;
	AcDbLine* m_LinePtr;	
	EPartOfArc m_whichPart; 

	//母线段，是弧？是圆？是多义线？:不可能是线line;  
	ETypeOfArc m_parentType ;
	AcDbArc* m_arcPtr;
	AcDbCircle* m_circlePtr;
	//AcDbLine* m_LinePtr;
	AcGeCircArc2d* m_circArc2d;
	AcDbPolyline* m_polylinePtr;

	//祖母线段，是弧？是线？是多义线? :若有，只能是多义线了; 
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
弧段提取线段CBreakArcToFourSegment;
将弧段分成四部分：以圆心画垂直线和水平线，将圆弧切割成四部分；
这样每部分都是单调的弧;
*/
class CBreakArcToFourPart
{
public:
	CBreakArcToFourPart(); 
	~CBreakArcToFourPart(); 
	CBreakArcToFourPart(AcGeCircArc2d* pGeArc2d);

private:
	AcGeCircArc2d* m_geArc2dToBreaked;   //需要打破的弧段;   
	AcGeCircArc2d* m_geArc2dLeftTop;
	AcGeCircArc2d* m_geArc2dLeftBot;  
	AcGeCircArc2d* m_geArc2dRightTop;  
	AcGeCircArc2d* m_geArc2dRightBot;   
	vector<AcGeCircArc2d*> m_arcs;    

private:
	bool breakArcToFourParts(); //也许打碎后小于4个部分;  

public:
	bool inputArcSegToBreaked(AcGeCircArc2d* pGeArc2d);  //输入需要打碎的弧段;    
	bool outputWhatPartAng(OUT AcGeCircArc2d* geArcPartOne,IN EPartOfArc whatPart); //输出第一象限弧段，如果有的话；
};



/*
class CBreakAcGeCircArcToTwoPart
功能：将输入的弧，按输入的角度，打破成2段弧;
输入：弧段，角度;
输出：打破后的2段弧;也可能角度不在弧段上，不打破;
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
功能：根据弧段部位，判断象限，得出象限最小角度，最大角度;
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
功能：从AcGeCircArc2d到AcDbArc2d转换;
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
功能：比较两个点的位置大小;
大小规则：在误差值equalPoint内，则判相等;超出者才比较大小;  
*/



/*
本项目的multiset的元素之间大小比较，结构;
这个用来比较事件点集合中的点，按y值从小到大排序；   
*/
struct eventPointCmp
{
	bool operator()(const SPointAndSegment& ps1,const SPointAndSegment& ps2)const  
	{
		double x1 = ps1.m_point.x; 
		double y1 = ps1.m_point.y;  
		double x2 = ps2.m_point.x;  
		double y2 = ps2.m_point.y;  

		//如果2个点距离不大于equalPoint()则判定相等;   
		if(ps1.m_point.distanceTo(ps2) <= AcGeTol::equalPoint())   
		{
			return false;  //如果相等，新插入的点往后排;
		}
		else
		{
			return (y1 > y2) || (y1 == y2 && x1 < x2);
		}
	}
}



/*
struct SPointAndSegment
功能：定义一个点和一个弧段的组合；
      用于事件点、扫描线上的弧段的结构的表达;
	  定义事件点的结构；
	  交点存储结构;
	  事件点存储于事件Q及扫描线想交线段结构T中；
*/
struct SPointAndSegment
{
	AcGePoint3d m_point;
	ELocationTypeOfPoint m_ePointLocation;  
	CSegement* m_segment;  
};




///*
//class CIntersectPoint;
//定义交点结构；
//交点为真正的交点，在扫描过程中发现的交点;   
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
Function:       // 函数名称
Description:    // 函数功能、性能等的描述
Calls:          // 被本函数调用的函数清单
Input:          // 输入参数说明，包括每个参数的作
                  // 用、取值说明及参数间关系。
Output:         // 对输出参数的说明。
Return:         // 函数返回值的说明
Others:         // 其它说明
*************************************************/
class CParseIntersectPoints
{
public:
	CParseIntersectPoints();  
	~CParseIntersectPoints();  

public:
	bool findIntersectPoints();  //发现所有交点;  	 
	bool outputIntersectPoints(OUT CSweeplinePointOpt* sweepLinePntsPtr);  
 
private: 
	bool findNewEventPoint();  
	bool parsePointLocationType();  
	bool sortByPointLocation(IN const <SPointAndSegment>& vecPoints);    

private:
	CIntersectPointOpt m_intsectPointsOpt;    
	CEventPointQueue m_eventPointsOpt;     
	CSweeplinePointOpt m_sweepOpt;   

	vector<SPointAndSegment> m_vEventPoints; //从Q中取出的事件点;    
	vector<SPointAndSegment> m_vSweepLinePoints; //从扫描线上取出来的“线段”;  

	vector<SPointAndSegment> m_vTopPoints; //上端点事件点集合; 
	vector<SPointAndSegment> m_vBottomPoints; //下端点事件点集合; 
	vector<SPointAndSegment> m_vMiddlePoints; //相交点集合(点在线段中间);   
	vector<SPointAndSegment> m_vecPointSegmentsNow; //当前事件点和扫描线点集合; 元素最少有1个;  

	//当前事件点的前一个事件点弧段； 和当前事件点的下一个事件点弧段；  --为了求交点（新事件）；
	SPointAndSegment m_preSegment;   
	vector<SPointAndSegment>  m_vecNextSegment;    //右边下一个弧段可能有多个;
	SPointAndSegment m_leftSegment;
	vector<SPointAndSegment > m_vecRightSegments; //最右边弧段，可能不止一个,如果不止一个，则都是平行的;

	int m_topPointsNum; //上端点事件点数量;    
	int m_botPointsNum; //下端点事件点数量;   
	int m_middlePointsNum; //相交点数量,也许只有一条弧段，有2条及以上弧段则说明是交点;

	AcGePoint3d m_curPoint; //当前坐标点;    
	//SPointAndSegment m_curPointAndSegment; //当前   

};


/*
class CEventPointOpt;
功能：事件队列上的操作；
是不是同样可以用于扫描线相交弧段的操作？？？  
*/
class CEventPointQueue
{
public:
	CEventPointOpt();   
	~CEventPointOpt();  

private:
	bool initSegmentsAll();  //初始化所有弧段;获取端点,插入事件队列;       
	bool insertEventPoint(IN const SPointAndSegment& eventPoint);   
	bool popOneEventPoint(OUT vector<SPointAndSegment>& vecEventPoints);    

private:
	multiset<SPointAndSegment,eventPointCmp> m_vEventPointsQueue; //事件点集合; 
};




/*
class CSweeplinePointOpt;
功能：事件队列上的操作；
*/
class CSweeplinePointOpt
{
public:
	CSweeplinePointOpt(); 
	~CSweeplinePointOpt();   

private:
	multiset<SPointAndSegment,eventPointCmp> m_vSweepLineSegments; //扫描线上的事件点（即弧段）集合;  
	vector<SPointAndSegment> m_vNowPointSegs;   
	AcGePoint3d m_point3dNow; //当前事件点;    

public:   
	bool insertSegment(IN SPointAndSegment& strPntSegPtr);  
	bool findPointSegments(IN AcGePoint3d pt,OUT vector<SPointAndSegment>& vStrPntSeg);    
	bool findPreviousSegment(OUT SPointAndSegment& prePointSegment); //发现最左边segmeng的左边segment； 
	bool findNextSegment(OUT SPointAndSegment& prePointSegment);   //发现最右边segmeng的右边segment； 
	bool findLeftSegmentOf(IN SPointAndSegment& refPointSegment,OUT SPointAndSegment& outPointSegment);   
	bool findRightSegmentOf(IN SPointAndSegment& pointSegment);  //发现最右边的segment;     
	bool deleteSegment(IN SPointAndSegment& pointSegment); 
	bool deleteSegments(IN vector<SPointAndSegment>& vSegments);   

public:
	bool ;
};




/*
class CIntersectPointOpt; 
功能：交点集合的操作；
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
	multiset<SPointAndSegment,eventPointCmp> m_intersectPoints;  //交点集合;
};




/*
class CParseIntersectPointByTwoSegment; 
功能：求俩弧段的交点，即新的事件点(交点类型);
注意：
1.如果2个segments已经有交点，则新的交点不能小于这个交点;
2.如果2个segments没有交点，则求的交点如何保证不是“旧交点”？
  解答：如果是旧交点，且已经经过事件处理,在已经发现的交点集合里应该存在;  
        如果是旧交点，还在事件集合里，事件集合不会重复插入;
3.这里只管计算出交点?
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
class COptOnPointsGroup; 
功能：对弧段（事件点）集合的一些操作;
注意：
1.如果2个segments已经有交点，则新的交点不能小于这个交点;
2.如果2个segments没有交点，则求的交点如何保证不是“旧交点”？
  解答：如果是旧交点，且已经经过事件处理,在已经发现的交点集合里应该存在;   
        如果是旧交点，还在事件集合里，事件集合不会重复插入;  
3.这里只管计算出交点?
*/
class COptOnPointsGroup
{
public:
	COptOnPointsGroup();  
	~COptOnPointsGroup();    
public:
	bool sortByAngle(IN OUT vector<SPointAndSegment>& vPoints); //按和扫描线离开后相交顺序排序;
	bool getLeftSegment(OUT vector<SPointAndSegment>& vPoints);  
	bool getRightSegment(OUT vector<SPointAndSegment>& vPoints);  
private:
	vector<SPointAndSegment> m_vPointsGroup;   
};


#endif  //ZHOUHUAGANG_20190822_segments_intersections