/************************************************************
Copyright (C), 2016-6-12, XuMiJieZi procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: Version :
Date:
Description:  1¡¢line,arcÂÖÀªÌáÈ¡£»

Version: // °æ±¾ÐÅÏ¢;
Function List: // Ö÷Òªº¯Êý¼°Æä¹¦ÄÜ
1. 
2.
3.
History: // ÀúÊ·ÐÞ¸Ä¼ÇÂ¼
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/


#include "segments_intersections.h"



/*------------------------------------------------------------
class CSegement
------------------------------------------------------------*/

//class CSegement::initCSegement
//¹¦ÄÜ£º1.½¨Á¢Ïß¶Î£º´Óline£¬arc£¬circle£¬polylineÌáÈ¡Ïß¶Î;
//2.³õÊ¼»¯Csegement;
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
	//m_x¸³Öµ
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
¹¦ÄÜ£º°Ñ»¡¶ÎÇÐ·Ö³ÉËÄ·Ý£¬Ã¿¸öÏóÏÞÒ»·Ý,Èç¹ûÓÐµÄ»°;
------------------------------------------------------------*/
/*
CBreakArcToFourPart::breakArcToFourParts()
·µ»ØÖµ£ºtrue-ÇÐ¸î³É¹¦£» false-Ã»ÓÐÇÐ¸î³É¹¦;
Ëã·¨£º1¡¢vectorÀïÃ¿¸ö»¡½øÐÐ0¶È¡¢90¶È¡¢180¶È¡¢270¶È·Ö¸î£»
      2¡¢Èç¹ûÈÎºÎÒ»´Î·Ö¸î³É¹¦£¬ÔòÔ­»¡É¾µô£¬·Ö¸îµÄÐÂ»¡¶Î¼ÓÈëvector£»
	  3¡¢Èç¹ûÃ»ÓÐÇÐ¸î³É¹¦£¬ÔòÔ­»¡²»É¾µô;
*/
bool CBreakArcToFourPart::breakArcToFourParts()
{
	AcGeCircArc2d* tmpGeArc2dPtr = NULL;
	AcGeCircArc2d* brkGeArc2dRslt1Ptr = NULL; //³õÊ¼»¯ºÜÖØÒª;
	AcGeCircArc2d* brkGeArc2dRslt2Ptr = NULL;
	//¶¨ÒåÒ»¸ö¶ÔÏó£º
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
				m_arcs.erase(itr); //itrÖ¸ÏòÏÂÒ»¸ö;
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


//¹¦ÄÜ£ºÅÐ¶ÏÄ³¸ö»¡¶ÎÊÇ·ñÔÚµÚÒ»ÏóÏÞ;Èç¹ûÔÚµÚÒ»ÏóÏÞ£¬Êä³öËü;
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
¹¦ÄÜ£º°´¸ø¶¨µÄ»¡¶È£¬°ÑÒ»¶Î»¡´òËéÎª2¶Î£»
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
//Îó²îÖµÒÔAcGeTol::equalPoint()Îª×¼;
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

	//ÅÐ¶Ï½Ç¶Èm_inputRadianToBreakÊÇ·ñÔÚ»¡ÄÚ;
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
¹¦ÄÜ£º¸ù¾Ý»¡¶Î²¿Î»£¬ÅÐ¶ÏÏóÏÞ£¬µÃ³öÏóÏÞ×îÐ¡½Ç¶È£¬×î´ó½Ç¶È;
*/
CQuadrantAnglePair::CQuadrantAnglePair()
{
}


CQuadrantAnglePair::~CQuadrantAnglePair()
{
}


//ÅÐ¶ÏÊÇÄÄ¸öÏóÏÞ£¬¼°ÏóÏÞµÄ×îÐ¡½ÇºÍ×î´ó½Ç¶È
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
¹¦ÄÜ£ºÉ¨ÃèÏßËã·¨£¬¼ÆËãËùÓÐ½»µã£»
-----------------------------------------------*/

CParseIntersectPoints::CParseIntersectPoints
{
	//clear
	m_vEventPoints.clear();
	m_vecNewEventPoints.clear();
}

CParseIntersectPoints::~CParseIntersectPoints()  
{
}


//ÇóËùÓÐ½»µã
bool 
CParseIntersectPoints::prsCurrentIntersectPoints()  
{
	//´ÓQÖÐµ¯³öµÚÒ»¸öÊÂ¼þµã,Ò»¸öÊÂ¼þµã¿ÉÄÜÓÐ¶à¸öÏàÍ¬µÄµã;	  
	m_eventPointsOpt.popOneEventPoint(m_vEventPoints);      

	//´ÓÉ¨ÃèÏßµ¯³öÊÂ¼þµã(ÕâÀï³Æ»¡¶Î);
	m_sweepOpt.popPointSegment(m_curPoint,m_vSweepLinePoints);     

	//¶ÔÉÏÁ©¼¯ºÏ½øÐÐ·ÖÀà;
	parsePointLocationType();   

	int numSegment = m_vecPointSegmentsNow.size();   
	if(numSegment > 1)  
	{
		findIntersectPoint();  //·¢ÏÖ½»µã;   
	}
	
	//É¾³ýÉ¨ÃèÏßÉÏµÄ»¡¶Î; É¨ÃèÏßÉÏµÄ»¡¶Î²»ÖØ¸´;µã¿ÉÒÔÖØ¸;  
	m_sweepOpt.deleteSegments(m_vBottomPoints);   
	m_sweepOpt.deleteSegments(m_vMiddlePoints);  
	//ÏòÉ¨ÃèÏß²åÈë»¡¶Î;
	m_sweepOpt.insertSegment(m_vTopPoints);     
	m_sweepOpt.insertSegment(m_vMiddlePoints);  
	//查找新的交点（事件点）
	m_sweepOpt.prsIntersectPoints();	
}



//功能：将事件点按端点位置类型分类;  
//分为：上端点，下端点，中间点（明显就是交点）；
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
			m_vMiddlePoints.push_back((SPointAndSegment)(*itr));   
			break;
		default:
			break;
		}
	}

	return true;
}


//·ÖÎöÏÖÓÐµÄÊÂ¼þµã¡¢É¨ÃèÏßµã£¬Çø·ÖÎªÉÏ¶ËµãÊ±¼ä£¬ÏÂ¶ËµãÊÂ¼þ£¬ÖÐ¼äµãÊ±¼ä
//È»ºó·Ö±ð´æ´¢£»
bool
CParseIntersectPoints::parsePointLocationType()
{
	sortByPointLocation();  
	sortByPointLocation();  
	return true;
}






/*-----------------------------------------------
class CSweeplinePointOpt
功能：扫描线类;
-----------------------------------------------*/
//find new event point
//1.当没有U(p)和C(p)元素个数时；2.当U(p)和C(p)元素个数不为0时;  
//此函数或许应该移动到扫描线类CSweeplinePointOpt； 
bool
CSweeplinePointOpt::prsIntersectPoints()  
{
	;
}



/*
class COptOnSegmentsGroup; 
功能：对多个事件点（有共同交点）进行操作；
      操作包括:找出最左边弧段、最右边弧段;
*/
COptOnSegmentsGroup::COptOnSegmentsGroup()
{
}

COptOnSegmentsGroup::~COptOnSegmentsGroup()
{
}


//sort by tangency angle
bool
COptOnSegmentsGroup::sortByAngle(IN OUT vector<SPointAndSegment>& vPoints)
{
	;
}




/*
class COptOnArc; 
给定弧段及其上一个点，计算其切线角度；
1.如果点在弧段中间，则有2个切角，一大一小，相差180度;
2.如果点在弧段端点处，则只有一个切角，切角指向另一端点;
*/
COptOnArc::COptOnArc()
{
	initMember();
}


COptOnArc::~COptOnArc()
{
}


COptOnArc::COptOnArc(IN const AcDbArc* pArc,IN AcGePoint3d midPoint) 
{	 	
	init(pArc,midPoint);
}


bool
COptOnArc::init(IN const AcDbArc* pArc,IN AcGePoint3d midPoint)
{
	initMember();
	bool isArcValid = false;
	bool isMidPointValid = false;
	isArcValid = init(pArc);
	isMidPointValid = init(midPoint);

	return (isMidPointValid && isMidPointValid);
}


bool
COptOnArc::init(IN const AcDbArc* pArc)
{
	initMember();
	if(pArc != NULL)
	{
		m_pArc = pArc;	
		getBaseInfoOfArc();
		return true;
	}
	return false;
}


bool
COptOnArc::init(IN const AcGePoint3d midPoint) 
{
	m_midPoint = midPoint; 
	if(isTheMidPointValid()) 
	{
		return true;
	}
	else
	{
		return false;
	}
}


void
COptOnArc::getBaseInfoOfArc()
{
	r = m_pArc->radius();
	m_startAngle = m_pArc->startAngle();
	m_endAngle = m_pArc->endAngle();
	m_centerPoint = m_pArc->center();
}


////判断弧段走向:AcDbArc都是逆时针的;  
//double
//COptOnArc::rtnBotPointTangency()  
//{	
//	findTopAndBotPoint();
//
//	if(isStartPointEqualToEndPoint())
//	{
//		return -1; //没法求得上端点和下端点;
//	}
//	else if(isTopPointEqualToStartPoint())
//	{
//		;
//	}
//}


//取起点坐标;
void
COptOnArc::calStartPoint()
{	
	AcGeVector2d vtrToStartPoint(r,0);
	vtrToStartPoint.rotateBy(m_startAngle);
	double xDlta = vtrToStartPoint.x;
	double yDlta = vtrToStartPoint.y;

	AcGeMatrix3d matrix3d;
	matrix3d.setToTranslation(vtrToStartPoint);  
	m_startPoint = m_centerPoint;   
	m_startPoint.transformBy(matrix3d); 	 
}


//
void
COptOnArc::calEndPoint()
{
	//取终点坐标;
	AcGeVector2d vtrToStartPoint(r,0);
	vtrToStartPoint.rotateBy(m_endAngle);
	double xDlta = vtrToStartPoint.x;
	double yDlta = vtrToStartPoint.y;

	AcGeMatrix3d matrix3d;
	matrix3d.setToTranslation(vtrToStartPoint);
	m_endPoint = m_centerPoint;
	m_endPoint.transformBy(matrix3d);  
}


//
//void
//COptOnArc::findTopAndBotPoint()
//{
//	if(isStartPointEqualToEndPoint())
//	{
//		m_topPoint = m_startPoint;
//		m_botPoint = m_startPoint;
//	}
//	else if(isTopPointEqualToStartPoint())
//	{
//		m_topPoint = m_startPoint;
//		m_botPoint = m_endPoint;
//	}
//	else
//	{
//		m_topPoint = m_endPoint;
//		m_botPoint = m_startPoint;
//	}
//}


//
double
COptOnArc::calStartPointTangency()
{
	AcGeVector2d vtr2d(r,0);
	vtr2d.rotateBy(m_startAngle);
	vtr2d.rotateBy(Pi* 0.5);
	m_startPointTangency = vtr2d.angle();

	//调整角度范围，保证在0-2π内;
	return m_startPointTangency;
}


//
double
COptOnArc::calEndPointTangency()
{
	AcGeVector2d vtr2d(r,0);
	vtr2d.rotateBy(m_endAngle);
	vtr2d.rotateBy(-Pi* 0.5);
	m_endPointTangency = vtr2d.angle();

	//调整角度范围，保证在0-2π内;
	return m_endPointTangency;
}


//
void
COptOnArc::isStartPointEqualToEndPoint()
{
	if(m_startPoint.isEqualTo(m_endPoint))
	{
		return true;
	}
	else
	{
		return false;
	}
}


//
bool
COptOnArc::isStartPointLowerThanEndPointInYcoord()
{
	double yStartPoint = m_startPoint.y;
	double yEndPoint = m_endPoint.y;
	if(yStartPoint - yEndPoint < AcGeTol::equalPoint())
	{
		return false;
	}
	else
	{
		return true;
	}
}


//
void
COptOnArc::initMember()
{
	m_pArc = NULL;   	
	r = -1;
	m_startAngle = -1;   
	m_endAngle = -1;
	m_startPointTangency = -1;     
	m_endPointTangency= -1;  	
}


//
bool
COptOnArc::isTheMidPointEqualStartPoint()
{
	if(m_midPoint.isEqualTo(m_startPoint))
	{
		return true;
	}
	else
	{
		return false;   
	}
}



//
bool
COptOnArc::isTheMidPointEqualEndPoint()
{
	if(m_midPoint.isEqualTo(m_endPoint))
	{
		return true;
	}
	else
	{
		return false;   
	}
}


//
bool
COptOnArc::isTheMidPointOnArc()
{
	m_pArc->isp
}


//在mid point处，有2个切线，也就是有2个切角;
bool
COptOnArc::calMidPointTangency()  
{
	if(!isTheMidPointValid())
	{
		return false;
	}
	else if(m_pArc == NULL)
	{
		return false;
	}	
	
	AcGeVector2d vtr2d;
	double x = m_midPoint.x - m_centerPoint.x;  
	double y = m_midPoint.y - m_centerPoint.y;   
	vtr2d.set(x,y);

	//tangency from midpoint pointing to endPoint
	vtr2d.rotateBy(Pi* 0.5);
	m_tanFromMidToEndPoint = vtr2d.angle();

	//tangency from midpoint pointing to startpoint;
	vtr2d.rotateBy(-1.0 * Pi);
	m_tanFromMidToStartPoint = vtr2d.angle();

	return true;
}


//check if the mid point is valid;
bool
COptOnArc::isTheMidPointValid()
{
	if(isTheMidPointEqualEndPoint())
	{
		return false;
	}
	else if(isTheMidPointEqualStartPoint())
	{
		return false;
	}
	else if(!isTheMidPointOnArc())
	{
		return false;
	}
	else
	{
		return true;  
	}
}


//
bool
COptOnArc::rtnTangencyFromMidToEndPoint(OUT double& dblTangency)
{	
	dblTangency = m_tanFromMidToEndPoint;
	if(m_tanFromMidToEndPoint != -1)
	{
		return true;
	}
	else  
	{
		return false;  
	}
}


//
bool
COptOnArc::rtnTangencyFromMidToStartPoint(OUT double& dblTangency)
{
	dblTangency = m_tanFromMidToStartPoint;
	if(m_tanFromMidToEndPoint != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}




/*
class COptOnLine; 
给定弧段及其上一个点，计算其切线角度；
1.如果点在弧段中间，则有2个切角，一大一小，相差180度;
2.如果点在弧段端点处，则只有一个切角，切角指向另一端点;
*/
COptOnLine::COptOnLine()
{
	initMember();
}


COptOnLine::~COptOnLine()
{
}


//
COptOnLine::COptOnLine(IN const AcDbLine* pLine)
{
	initMember();
	init(pLine);
}


//
COptOnLine::COptOnLine(IN AcGePoint3d midPoint)  
{
	init(m_midPoint);
}


COptOnLine::COptOnLine(IN const AcDbLine* pLine,IN AcGePoint3d midPoint)  
{
	initMember();
	init(pLine,midPoint);  
}


bool
COptOnLine::init(IN const AcDbArc* pArc,IN AcGePoint3d midPoint)  
{
	bool isArcValid = false;
	bool isMidPointValid = false; 
	isArcValid = init(pArc);
	isMidPointValid = init(midPoint);

	return (isMidPointValid && isMidPointValid);
}


bool
COptOnLine::init(IN const AcDbLine* pLine)
{
	if(pLine != NULL)
	{
		initMember();
		m_pLine = pLine;
		getBaseInfoOfArc();
		return true;
	}
	return false;
}


//
void
COptOnLine::init(IN AcGePoint3d midPoint)
{
	m_midPoint = midPoint;
}


//
void
COptOnLine::initMember()
{
	m_pLine = NULL;   			
	//double m_startAngle;   
	//double m_endAngle;   

	//AcGePoint3d m_midPoint; 
	//AcGePoint3d m_startPoint;   
	//AcGePoint3d m_endPoint;
	m_startPointTangency = -1;   
	m_endPointTangency = -1; 
	//double m_tanFromMidToStartPoint;    
	//double m_tanFromMidToEndPoint;	
}

void
COptOnLine::getBaseInfoOfArc()
{
	m_startPoint = m_pLine->startPoint();  
	m_endPoint = m_pLine->endPoint();   
}


//
double
COptOnLine::calStartPointTangency()
{
	double xStart = m_startPoint.x;
	double yStart = m_startPoint.y;
	double xEnd = m_endPoint.x;     
	double yEnd = m_endPoint.y;

	AcGeVector2d vtr2d(xEnd-xStart,yEnd-yStart);
	m_startPointTangency = vtr2d.angle();  

	//调整角度范围，保证在0-2π内;--不需要调整;
	return m_startPointTangency;
}


//
double
COptOnLine::calEndPointTangency()
{
	double xStart = m_startPoint.x;
	double yStart = m_startPoint.y;
	double xEnd = m_endPoint.x;
	double yEnd = m_endPoint.y;

	AcGeVector2d vtr2d(xStart-xEnd,yStart-yEnd);
	m_endPointTangency = vtr2d.angle();  

	//调整角度范围，保证在0-2π内;--不需要调整;
	return m_endPointTangency;
}


//
bool
COptOnLine::isTheMidPointEqualEndPoint()
{
	if(m_midPoint.isEqualTo(m_endPoint))
	{
		return true;
	}
	else
	{
		return false;   
	}
}


//
bool
COptOnLine::isStartPointLowerThanEndPointInYcoord()
{
	double yStartPoint = m_startPoint.y;
	double yEndPoint = m_endPoint.y;
	if(yStartPoint - yEndPoint < AcGeTol::equalPoint())
	{
		return false;
	}
	else
	{
		return true;
	}
}


//
bool
COptOnLine::isTheMidPointEqualStartPoint()
{
	if(m_midPoint.isEqualTo(m_startPoint))
	{
		return true;
	}
	else
	{
		return false;   
	}
}


//check if the mid point is valid;
bool
COptOnLine::isTheMidPointValid()
{
	if(isTheMidPointEqualEndPoint())
	{
		return false;
	}
	else if(isTheMidPointEqualStartPoint())
	{
		return false;
	}
	else if(!isTheMidPointOnArc())
	{
		return false;
	}
	else
	{
		return true;  
	}
}


//
double
COptOnLine::rtnTangencyFromMidToEndPoint(OUT double& dblTangency)
{	
	return m_startPointTangency;
}


//
double
COptOnLine::rtnTangencyFromMidToStartPoint(OUT double& dblTangency)
{
	return m_endPointTangency;
}


//
double COptOnLine::rtnTangencyFromStartPoint()
{
	return m_startPointTangency;
}


//
double
COptOnLine::rtnTangencyFromEndPoint()
{
	return m_endPointTangency;
}




/*
class ConvertArcToArc
功能：AcGeCircArc3d和AcDbArc互相转换；
*/
//
ConvertArcToArc::ConvertArcToArc()
{
}


//
ConvertArcToArc::~ConvertArcToArc()
{
}


//注意：有用户内存分配
void 
ConvertArcToArc::convertArc2Arc(IN AcGeCircArc3d*pGeArc, OUT AcDbArc*&pDbArc)
{
	AcGePoint3d center = pGeArc->center();
	AcGeVector3d normal = pGeArc->normal();
	AcGeVector3d refVec = pGeArc->refVec();
	AcGePlane plane = AcGePlane(center, normal);
	double ang = refVec.angleOnPlane(plane);
	pDbArc = new AcDbArc(center, normal,
		                 pGeArc->radius(),
						 pGeArc->startAng() + ang,
						 pGeArc->endAng() + ang );
}


//注意：有用户内存分配
void 
ConvertArcToArc::convertArc2Arc(IN AcDbArc*pDbArc, OUT AcGeCircArc3d*&pGeArc)

{
	pGeArc = new AcGeCircArc3d(pDbArc->center(),
							   pDbArc->normal(),
							   pDbArc->normal().perpVector(),
							   pDbArc->radius(),
							   pDbArc->startAngle(),
							   pDbArc->endAngle());
}



/*
class CParsTangencyOnArc; 
给定弧段及其上一个点，计算其切线角度；
1.如果点在弧段中间，则有2个切角，一大一小，相差180度;
2.如果点在弧段端点处，则只有一个切角，切角指向另一端点;
*/
CParsTangencyOnArc::CParsTangencyOnArc()
{
	initMember();
}

CParsTangencyOnArc::~CParsTangencyOnArc()
{
	delete m_pArcOpt;
}


//
CParsTangencyOnArc::CParsTangencyOnArc(IN AcDbArc* pArc)
{
	m_pArcOpt->init(pArc);
}


//
CParsTangencyOnArc::CParsTangencyOnArc(IN AcGePoint3d midPoint) 
{
	m_pArcOpt->init(midPoint);
}


//
CParsTangencyOnArc::CParsTangencyOnArc(IN AcDbArc* pArc,IN AcGePoint3d midPoint)
{
	initMember();
	m_pArcOpt->init(pArc,midPoint);
}



//
bool
CParsTangencyOnArc::init(IN AcDbArc* pArc)
{
	initMember();	
	return m_pArcOpt->init(pArc);
}


//
bool
CParsTangencyOnArc::init(IN AcDbArc* pArc,IN AcGePoint3d midPoint)
{
	initMember();
	return m_pArcOpt->init(pArc,midPoint);
}


//
bool
CParsTangencyOnArc::init(IN AcGePoint3d midPoint)
{
	return m_pArcOpt->init(midPoint);
}


//
void
CParsTangencyOnArc::initMember()
{	
	COptOnArc* m_pArcOpt = NULL;  	    
	if(m_pArcOpt == NULL)
	{
		m_pArcOpt = new COptOnArc;
	}
	m_topPointTangency = -1;   
	m_botPointTangency = -1;   
	m_midToBotTangency = -1;  
	m_midToTopTangency = -1;   
}



//
void
CParsTangencyOnArc::calTopAndBotPointTangency()  
{
	m_pArcOpt->calStartPointTangency();  
	m_pArcOpt->calEndPointTangency();  
	//此时可以计算2个Tangency
	if(m_pArcOpt->isStartPointLowerThanEndPointInYcoord())
	{
		m_botPointTangency = m_pArcOpt->rtnTangencyFromStartPoint();
		m_topPointTangency = m_pArcOpt->rtnTangencyFromEndPoint();
	}
	else
	{
		m_botPointTangency = m_pArcOpt->rtnTangencyFromEndPoint();
		m_topPointTangency = m_pArcOpt->rtnTangencyFromStartPoint();		
	}	
}



//
void
CParsTangencyOnArc::calMidToTopAndBotTangency()
{
	if(m_pArcOpt->isStartPointLowerThanEndPointInYcoord())
	{
		m_midToBotTangency = m_pArcOpt->rtnTangencyFromMidToStartPoint();
		m_midToTopTangency = m_pArcOpt->rtnTangencyFromMidToEndPoint();
	}
	else
	{
		m_midToBotTangency = m_pArcOpt->rtnTangencyFromMidToEndPoint();
		m_midToTopTangency = m_pArcOpt->rtnTangencyFromMidToStartPoint();
	}
}


//
//bool
//CParsTangencyOnArc::calMidToTopTangency()
//{
//	;
//}