/************************************************************
Copyright (C), 2016-6-12, XuMiJieZi procision maskmaking. Co., Ltd.
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


#include "segments_intersections.h"



/*------------------------------------------------------------
class CSegement
------------------------------------------------------------*/

//class CSegement::initCSegement
//功能：1.建立线段：从line，arc，circle，polyline提取线段;
//2.初始化Csegement;
bool
CSegement::initCSegement(IN AcDbLine* linePtr)
{
	double x1;
	double y1;
	double x2;
	double y2;
	x1 = linePtr->startPoint().x;
	y1 = linePtr->startPoint().y;
	x2 = linePtr->endPoint().x;
	y2 = linePtr->endPoint().y;
	s
	//m_x赋值
	if(y1 > y2)
	{
		m_xTop = x1;
		m_yTop = y1;
		m_xBottom = x2;
		m_yBottom = y2;
	}
	else
	{
		m_xTop = x2;
		m_yTop = y2;
		m_xBottom = x1;
		m_yBottom = y2;
	}

	//self
	m_myselfType = Line_Type;
	m_arcPtr = NULL;
	m_circlePtr = NULL;
	m_LinePtr = linePtr;

	//mather
	m_parentType =  None_Type;
	m_arcPtr = NULL;
	AcDbCircle* m_circlePtr = NULL;
	//AcDbLine* m_LinePtr;
	AcGeCircArc2d* m_circArc2d = NULL;
	AcDbPolyline* m_polylinePtr = NULL;

	//grandmather
	ETypeOfArc m_grandmaType = None_Type;
	//AcDbArc* m_arcPtr;
	//AcDbLine* m_LinePtr;
	AcDbPolyline* m_polylinePtr = NULL;
	
	return true;
}




/*------------------------------------------------------------
class CBreakArcToFourPart
功能：把弧段切分成四份，每个象限一份,如果有的话;
------------------------------------------------------------*/
/*
CBreakArcToFourPart::breakArcToFourParts()
返回值：true-切割成功； false-没有切割成功;
算法：1、vector里每个弧进行0度、90度、180度、270度分割；
      2、如果任何一次分割成功，则原弧删掉，分割的新弧段加入vector；
	  3、如果没有切割成功，则原弧不删掉;
*/
bool CBreakArcToFourPart::breakArcToFourParts()
{
	AcGeCircArc2d* tmpGeArc2dPtr = NULL;
	AcGeCircArc2d* brkGeArc2dRslt1Ptr = NULL; //初始化很重要;
	AcGeCircArc2d* brkGeArc2dRslt2Ptr = NULL;
	//定义一个对象：
	CBreakAcGeCircArcToTwoPart objBrkTwo;

	int i = 0;
	double tmpRadian = 0;
	vector<AcGeCircArc2d*>::iterator itr = m_arcs.begin();   
	for(; itr != m_arcs.end(); itr++)
	{
		tmpGeArc2dPtr = *itr; 
		//cut four times
		for(int i = 0; i < 4; i++)
		{
			tmpRadian = (Pi / 2.0) * i;
			objBrkTwo.breakArc(m_geArc2dToBreaked,tmpRadian,brkGeArc2dRslt1Ptr,brkGeArc2dRslt2Ptr);
			if(brkGeArc2dRslt1Ptr != NULL || brkGeArc2dRslt2Ptr != NULL)
			{
				if(brkGeArc2dRslt1Ptr != NULL)
				{
					m_arcs.push_back(brkGeArc2dRslt1Ptr); 
				}
				if(brkGeArc2dRslt2Ptr != NULL)
				{
					m_arcs.push_back(brkGeArc2dRslt2Ptr);  
				}
				m_arcs.erase(itr); //itr指向下一个;
				break;
			}
			else
			{
				continue;
			}			
		}		
	}
}


//input one arc,initialize
bool
CBreakArcToFourPart::inputArcSegToBreaked(AcGeCircArc2d* pGeArc2d)
{
	if(pGeArc2d != NULL)
	{
		m_geArc2dToBreaked = pGeArc2d;   
		return true;
	}
	else
	{
		return false;
	}
}


//功能：判断某个弧段是否在第一象限;如果在第一象限，输出它;
bool 
CBreakArcToFourPart::outputWhatPartAng(OUT AcGeCircArc2d* geArcPartOne,IN EPartOfArc whatPart)
{
	double tmpStartAng = -1;
	double tmpEndAng = -1;
	double startQuadrantAngle = -1;
	double endQuadrantAngle = -1;
	CQuadrantAnglePair objQuadrantAngle;
	if(!objQuadrantAngle.getQuadrantAnglePair(whatPart,startQuadrantAngle,endQuadrantAngle))
	{
		return false;
	}

	AcGeCircArc2d* tmpCircArc2dPtr = NULL;
	vector<AcGeCircArc2d*>::iterator itr = m_arcs.begin();  
	for(; itr != m_arcs.end(); itr++)
	{
		tmpCircArc2dPtr = *itr;
		tmpStartAng = tmpCircArc2dPtr->startAng();
		tmpEndAng = tmpCircArc2dPtr->endAng();
		if((tmpStartAng >= (startQuadrantAngle + AcGeTol::equalPoint())) && (tmpEndAng < (endQuadrantAngle + AcGeTol::equalPoint())) )
		{
			geArcPartOne = *itr;
			return true
		}
	}
	return false;
}




/*-----------------------------------------------
class CBreakAcGeCircArcToTwoPart
功能：按给定的弧度，把一段弧打碎为2段；
-----------------------------------------------*/
CBreakAcGeCircArcToTwoPart::CBreakAcGeCircArcToTwoPart()
{
}


CBreakAcGeCircArcToTwoPart::~CBreakAcGeCircArcToTwoPart()
{
	m_inputArc = NULL;
	m_arcResult1 = NULL;
	m_arcResult2 = NULL;
}



//initialize the member variables;
bool
CBreakAcGeCircArcToTwoPart::setArcAndRadianToBreak(IN AcGeCircArc2d* pArc,IN double radianToBrk)
{
	bool b1 = false;
	bool b2 = false;
	b1 = setArcToBeBrked(pArc);  
	b2 = setRadianToBeBrked(radianToBrk);  
	return (b1 && b2);
}


//initialize
bool
CBreakAcGeCircArcToTwoPart::setArcToBeBrked(IN AcGeCircArc2d* pArc)
{
	m_inputArc = pArc;
	if(pArc == NULL)
	{		
		return false;
	}
	else
	{	
		return true;
	}
}



//initialize
//误差值以AcGeTol::equalPoint()为准;
bool
CBreakAcGeCircArcToTwoPart::setRadianToBeBrked(IN double radianToBrk)
{
	m_inputRadianToBreak = radianToBrk;
	if((radianToBrk < -AcGeTol::equalPoint()) && (radianToBrk >= (2*Pi -AcGeTol::equalPoint())))
	{		
		return false;
	}
	else
	{	
		return true;
	}
}


//break the input arc
bool
CBreakAcGeCircArcToTwoPart::breakArc()
{
	double startAng = m_inputArc->startAng();   
	double endAng = m_inputArc->endAng();     
	double radius = m_inputArc->radius();     
	AcGePoint2d centerPt = m_inputArc->center(); 

	//判断角度m_inputRadianToBreak是否在弧内;
	if(m_inputArc > (startAng + AcGeTol::equalPoint())) 
	{ 
		if(m_inputArc < (endAng - AcGeTol::equalPoint()))  
		{
			m_arcResult1 = new AcGeCircArc2d;
			m_arcResult1->setRadius(radius);
			m_arcResult1->setAngles(startAng,m_inputArc);  
			m_arcResult1->setCenter(centerPt);

			m_arcResult2 = new AcGeCircArc2d;
			m_arcResult2->setRadius(radius);
			m_arcResult2->setAngles(m_inputArc,endAng);  
			m_arcResult2->setCenter(centerPt);

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}



//output the result arcs 
bool
CBreakAcGeCircArcToTwoPart::outputBreakedArcs(OUT AcGeCircArc2d* pArcResult1,OUT AcGeCircArc2d* pArcResult2)
{
	bool b1 = false;
	bool b2 = false;
	if(m_arcResult1 != NULL)
	{
		pArcResult1 = m_arcResult1;
		b1 = true;
	}

	if(m_arcResult2 != NULL)
	{
		pArcResult2 = m_arcResult2;
		b2 = true;
	}

	return (b1 || b2);
}


//
bool 
CBreakAcGeCircArcToTwoPart::breakArc(IN AcGeCircArc2d* pArc,IN double radianToBrk,OUT AcGeCircArc2d* pArcResult1,OUT AcGeCircArc2d* pArcResult2)
{
	if(!setArcToBeBrked(pArc))
	{
		return false;
	}

	if(!setRadianToBeBrked(radianToBrk))
	{
		return false;
	}

	if(!breakArc())
	{
		return false;
	}

	if(!outputBreakedArcs(pArcResult1,pArcResult2))
	{
		return false;
	}

	return true;
}




/*
class CQuadrantAnglePair
功能：根据弧段部位，判断象限，得出象限最小角度，最大角度;
*/
CQuadrantAnglePair::CQuadrantAnglePair()
{
}


CQuadrantAnglePair::~CQuadrantAnglePair()
{
}


//判断是哪个象限，及象限的最小角和最大角度
bool
CQuadrantAnglePair::getQuadrantAnglePair(IN EPartOfArc whichPart,OUT double& startAngle,OUT double& endAngle)
{
	int n = -1;
	switch(whichPart)
	{
	case LEFT_BOTTOM:
		n = 2;
		break;
	case LEFT_TOP:
		n = 1;
		break;
	case RIGHT_BOTTOM:
		n = 3;
		break;
	case RIGHT_TOP:
		n = 0;
		break;
	default:
		break;
	}

	if(n >= 0 && n < 4)
	{
		startAngle = n*Pi/2;
		endAngle = (n+1)*Pi/2;
	}
	else
	{
		return false;
	}
	
}




/*-----------------------------------------------
class CParseIntersectPoints
功能：扫描线算法，计算所有交点；
-----------------------------------------------*/

CParseIntersectPoints::CParseIntersectPoints
{
}

CParseIntersectPoints::~CParseIntersectPoints()  
{
}


//求所有交点
bool 
CParseIntersectPoints::findIntersectPoints()  
{
	//从Q中弹出第一个事件点,一个事件点可能有多个相同的点;	  
	m_eventPointsOpt.popOneEventPoint(m_vEventPoints);      

	//从扫描线弹出事件点(这里称弧段);
	m_sweepOpt.popPointSegment(m_curPoint,m_vSweepLinePoints);     

	//对上俩集合进行分类;
	parsePointLocationType();   

	int numSegment = m_vecPointSegmentsNow.size();   

	if(numSegment > 1)  
	{
		findIntersectPoint();  //发现交点;   
	}

	//检查上端点事件和中端点事件个数；
	//m_topPointsNum = m_vTopPoints.size();   
	//m_botPointsNum = m_vBottomPoints.size();    
	//m_middlePointsNum = m_vMiddlePoints.size();     
	if(m_topPointsNum == 0 && m_middlePointsNum == 0) //如果没有上端点事件，也没有中间点事件   
	{
		m_sweepOpt.deleteSegments(m_vBottomPoints);  
		m_sweepOpt.deleteSegments(m_vMiddlePoints); 
		m_sweepOpt.insertSegment(m_vTopPoints);
		m_sweepOpt.insertSegment(m_vMiddlePoints);
		//find new intersectpoints
		findLeftSegments();
		findRightSegments();
		findFrontSegments();
		findBehindSegments();		
		//测试有无交点，有交点，则纳入事件点Q   
		calNewEventPoints();   
		insertNewEventPoints();    
	}
	else  //
	{
		//出入U(p)和C（p)中的线段，按逆序，也即按扫描线离开后的线段顺序；
		vector<SPointAndSegment> vecUpAndCross;
		vecUpAndCross.insert(m_vTopPoints.begin(),m_vTopPoints.end());   
		vecUpAndCross.insert(m_vMiddlePoints.begin(),m_vMiddlePoints.end());  
		COptOnPointsGroup optOnPnGrp;
		optOnPnGrp.sortByAngle(vecUpAndCross);
		//测试有无交点，有交点，则纳入事件点Q  
		
	}

	//检查下端点个数;
}



//分析现有的事件点、扫描线点，区分为上端点时间，下端点事件，中间点时间  
//然后分别存储；
bool
CParseIntersectPoints::sortByPointLocation(IN const <SPointAndSegment>& vecPoints)   
{
	vector<SPointAndSegment>::iterator itr = vecPoints.begin();
	for(; itr != vecPoints.end(); itr++)
	{
		switch(itr->m_ePointLocation)
		{
		case ELocationTypeOfPoint::TOP_POINT:
		case ELocationTypeOfPoint::LEFT_POINT:
			m_vTopPoints.push_back((SPointAndSegment)(*itr));
			break;
		case ELocationTypeOfPoint::BOT_POINT:
		case ELocationTypeOfPoint::RIGHT_POINT:
			m_vBottomPoints.push_back((SPointAndSegment)(*itr));
			break;
		case ELocationTypeOfPoint::MIDDLE_POINT:
			m_vIntersectPoints.push_back((SPointAndSegment)(*itr));   
			break;
		default:
			break;
		}
	}

	return true;
}


//分析现有的事件点、扫描线点，区分为上端点时间，下端点事件，中间点时间
//然后分别存储；
bool
CParseIntersectPoints::parsePointLocationType()
{
	sortByPointLocation(m_eventPointsOpt);
	sortByPointLocation(m_sweepOpt);
	return true;
}