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
class CPrsTangencyOfArc; 
给定弧段及其上一个点，计算其切线角度；
1.如果点在弧段中间，则有2个切角，一大一小，相差180度;
2.如果点在弧段端点处，则只有一个切角，切角指向另一端点;
*/

CPrsTangencyOfArc::CPrsTangencyOfArc()
{
	m_pArc = NULL;
	m_startAngle = -1;
	m_endAngle = -1;
	m_botPointTangency = -1;	
	m_topPointTangency = -1;
	m_tanFromMidToBot = -1;
	m_tanFromMidToTop = -1;
}

CPrsTangencyOfArc::~CPrsTangencyOfArc()
{
}


CPrsTangencyOfArc::CPrsTangencyOfArc(IN const AcDbArc* pArc,IN AcGePoint3d midPoint) 
{	 	
	return init(pArc,midPoint);
}


bool
CPrsTangencyOfArc::init(IN const AcDbArc* pArc,IN AcGePoint3d midPoint)
{
	if(pArc != NULL)
	{
		m_pArc = pArc;
		m_midPoint = midPoint;
		return true;
	}
	return false;
}


bool
CPrsTangencyOfArc::init(IN const AcDbArc* pArc)
{
	if(pArc != NULL)
	{
		m_pArc = pArc;		
		return true;
	}
	return false;
}


void
CPrsTangencyOfArc::init(IN const AcGePoint3d midPoint) 
{
	m_midPoint = midPoint; 
}


//判断弧段走向:AcDbArc都是逆时针的;  
double
CPrsTangencyOfArc::calBotPointTangency()  
{
	double r = m_pArc->radius();
	double startAngle = m_pArc->startAngle();  
	double endAngle = m_pArc->endAngle();  
	m_centerPoint = m_pArc->center();  	
	//哪个角度是bottom point的角度（圆心到端点）？ 
	
	AcGeVector2d vtrToStartPoint(r,0);
	vtrToStartPoint.rotateBy(m_startAngle);
	double xDlta = vtrToStartPoint.x;
	double yDlta = vtrToStartPoint.y;
	
	AcGeMatrix3d matrix3d;
	matrix3d.setToTranslation(vtrToStartPoint);  
	AcGePoint3d startPoint = m_centerPoint;   
	startPoint.transformBy(matrix3d);   

	;

}