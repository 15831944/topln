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
	//·¢ÏÖ×óÓÒ»¡¶Î;
	findLeftSegments();   
	findRightSegments();   
	findFrontSegments();     
	findBehindSegments();		   
	//²âÊÔÓÐÎÞ½»µã£¬ÓÐ½»µã£¬ÔòÄÉÈëÊÂ¼þµãQ      
	calNewEventPoints();   
	insertNewEventPoints();   
}



//·ÖÎöÏÖÓÐµÄÊÂ¼þµã¡¢É¨ÃèÏßµã£¬Çø·ÖÎªÉÏ¶ËµãÊ±¼ä£¬ÏÂ¶ËµãÊÂ¼þ£¬ÖÐ¼äµãÊ±¼ä  
//È»ºó·Ö±ð´æ´¢£»
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


//find new event point
//1.当没有U(p)和C(p)元素个数时；2.当U(p)和C(p)元素个数不为0时; 
bool
CParseIntersectPoints::calNewEventPoints() 
{
	;
}