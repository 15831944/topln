/************************************************************
Copyright (C), 2016-6-12, XuMiJieZi procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: Version :
Date:
Description:  1��line,arc������ȡ��

Version: // �汾��Ϣ;
Function List: // ��Ҫ�������书��
1. 
2.
3.
History: // ��ʷ�޸ļ�¼
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/


#include "segments_intersections.h"



/*------------------------------------------------------------
class CSegement
------------------------------------------------------------*/

//class CSegement::initCSegement
//���ܣ�1.�����߶Σ���line��arc��circle��polyline��ȡ�߶�;
//2.��ʼ��Csegement;
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
	//m_x��ֵ
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
���ܣ��ѻ����зֳ��ķݣ�ÿ������һ��,����еĻ�;
------------------------------------------------------------*/
/*
CBreakArcToFourPart::breakArcToFourParts()
����ֵ��true-�и�ɹ��� false-û���и�ɹ�;
�㷨��1��vector��ÿ��������0�ȡ�90�ȡ�180�ȡ�270�ȷָ
      2������κ�һ�ηָ�ɹ�����ԭ��ɾ�����ָ���»��μ���vector��
	  3�����û���и�ɹ�����ԭ����ɾ��;
*/
bool CBreakArcToFourPart::breakArcToFourParts()
{
	AcGeCircArc2d* tmpGeArc2dPtr = NULL;
	AcGeCircArc2d* brkGeArc2dRslt1Ptr = NULL; //��ʼ������Ҫ;
	AcGeCircArc2d* brkGeArc2dRslt2Ptr = NULL;
	//����һ������
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
				m_arcs.erase(itr); //itrָ����һ��;
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


//���ܣ��ж�ĳ�������Ƿ��ڵ�һ����;����ڵ�һ���ޣ������;
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
���ܣ��������Ļ��ȣ���һ�λ�����Ϊ2�Σ�
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
//���ֵ��AcGeTol::equalPoint()Ϊ׼;
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

	//�жϽǶ�m_inputRadianToBreak�Ƿ��ڻ���;
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
���ܣ����ݻ��β�λ���ж����ޣ��ó�������С�Ƕȣ����Ƕ�;
*/
CQuadrantAnglePair::CQuadrantAnglePair()
{
}


CQuadrantAnglePair::~CQuadrantAnglePair()
{
}


//�ж����ĸ����ޣ������޵���С�Ǻ����Ƕ�
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
���ܣ�ɨ�����㷨���������н��㣻
-----------------------------------------------*/

CParseIntersectPoints::CParseIntersectPoints
{
}

CParseIntersectPoints::~CParseIntersectPoints()  
{
}


//�����н���
bool 
CParseIntersectPoints::findIntersectPoints()  
{
	//��Q�е�����һ���¼���,һ���¼�������ж����ͬ�ĵ�;	  
	m_eventPointsOpt.popOneEventPoint(m_vEventPoints);      

	//��ɨ���ߵ����¼���(����ƻ���);
	m_sweepOpt.popPointSegment(m_curPoint,m_vSweepLinePoints);     

	//���������Ͻ��з���;
	parsePointLocationType();   

	int numSegment = m_vecPointSegmentsNow.size();   

	if(numSegment > 1)  
	{
		findIntersectPoint();  //���ֽ���;   
	}

	//����϶˵��¼����ж˵��¼�������
	//m_topPointsNum = m_vTopPoints.size();   
	//m_botPointsNum = m_vBottomPoints.size();    
	//m_middlePointsNum = m_vMiddlePoints.size();     
	if(m_topPointsNum == 0 && m_middlePointsNum == 0) //���û���϶˵��¼���Ҳû���м���¼�   
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
		//�������޽��㣬�н��㣬�������¼���Q   
		calNewEventPoints();   
		insertNewEventPoints();    
	}
	else  //
	{
		//����U(p)��C��p)�е��߶Σ�������Ҳ����ɨ�����뿪����߶�˳��
		vector<SPointAndSegment> vecUpAndCross;
		vecUpAndCross.insert(m_vTopPoints.begin(),m_vTopPoints.end());   
		vecUpAndCross.insert(m_vMiddlePoints.begin(),m_vMiddlePoints.end());  
		COptOnPointsGroup optOnPnGrp;
		optOnPnGrp.sortByAngle(vecUpAndCross);
		//�������޽��㣬�н��㣬�������¼���Q  
		
	}

	//����¶˵����;
}



//�������е��¼��㡢ɨ���ߵ㣬����Ϊ�϶˵�ʱ�䣬�¶˵��¼����м��ʱ��  
//Ȼ��ֱ�洢��
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


//�������е��¼��㡢ɨ���ߵ㣬����Ϊ�϶˵�ʱ�䣬�¶˵��¼����м��ʱ��
//Ȼ��ֱ�洢��
bool
CParseIntersectPoints::parsePointLocationType()
{
	sortByPointLocation(m_eventPointsOpt);
	sortByPointLocation(m_sweepOpt);
	return true;
}