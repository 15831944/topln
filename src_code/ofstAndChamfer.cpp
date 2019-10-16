/************************************************************
Copyright (C), 2016-6-12, XuMiJieZi procision maskmaking. Co., Ltd.
FileName: ofstAndChamfer.cpp
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
owal  dec-13 2017 1.0  offset and chamfer
***********************************************************/
#include "..\StdAfx.h"
#include  "ofstAndChamfer.h"
#include "selectss.h"
#include "TCHAR.h"


//=============================
//struct  STopY
//=============================
//copy constructor;
STopY::STopY(const STopY& objSrc)
{
	m_iIndexVtx = objSrc.m_iIndexVtx;
	m_pt2d = objSrc.m_pt2d;
	m_topYLocation = objSrc.m_topYLocation; 
	m_y = objSrc.m_y;
}


//
STopY::STopY()
{
}


//
STopY::~STopY()
{
}



//=============================
//class CPlineOffset
//=============================
CPlineOffset::CPlineOffset()
{
	m_offsetDist = 0;
}


//destructor
CPlineOffset::~CPlineOffset()
{
}


//
double
CPlineOffset::setOffsetDist(IN const double dblDist)
{
	m_offsetDist = dblDist;  
	return m_offsetDist;  
}


//��ĳ������ִ��offset
void
CPlineOffset::ofst(IN AcDbPolyline* pPlineOrg,OUT AcDbVoidPtrArray& m_plineArr)
{
	if(m_offsetDist == 0)
	{
		return;  
	}

	if(!checkPolyline(pPlineOrg)) //�����߲��ϸ񣬷��������;    
	{
		addLayer(_T("problem"));  
		pPlineOrg->setLayer(_T("problem"));  
		return;
	}

	//ȡextent;
	AcDbExtents orgExt;
	AcDbExtents newExt;
	pPlineOrg->getGeomExtents(orgExt);

	//�ж϶������Ƿ���ʱ��;
	double ofstVal = m_offsetDist / 2.0;  
	CJudgePlineCCW objPlCcw(pPlineOrg);
	if(objPlCcw.isPlineCCW() == CLOCKWISE_PLINE)
	{
		ofstVal = -ofstVal;
	}

	//offset	
	AcDbVoidPtrArray rslArray;
	Acad::ErrorStatus es =  pPlineOrg->getOffsetCurves(ofstVal,rslArray);
	if(es != Acad::eOk)
	{
		//offsetʧ�ܣ�����ʵ����롰problem����;  
		addLayer(_T("problem"));
		pPlineOrg->setLayer(_T("problem"));
		return;
	}
	else
	{
		//��offset�������array;
		int nNum = rslArray.length();  
		if(nNum == 0)
		{
			addLayer(_T("problem"));
			pPlineOrg->upgradeOpen();
			pPlineOrg->setLayer(_T("problem"));  
			pPlineOrg->downgradeOpen();  
			return;
		}

		//�ж�offset����С�Ƿ���ȷ;
		CExtentOfSS objExt;
		objExt.getExtents(rslArray,newExt); 
		bool bFlag = false;
		bFlag = checkOffsetRsltSize(orgExt,newExt,m_offsetDist);
		if(bFlag)
		{
			for(int i = 0; i < nNum; i++)
			{
				void* pVoid = rslArray.at(i);   
				m_plineArr.append(pVoid);   
			}

			//ɾ��������;
			pPlineOrg->upgradeOpen();  
			pPlineOrg->erase();
			pPlineOrg->downgradeOpen();  
		}
		else //�������Ķ����߼����;
		{
			addLayer(_T("problem"));
			pPlineOrg->upgradeOpen();
			pPlineOrg->setLayer(_T("problem"));  
			pPlineOrg->downgradeOpen();  
			return;
		}
	}
}


//offset the circle
void
CPlineOffset::ofst(IN AcDbCircle* pCircle)
{
	if(pCircle == NULL)
	{
		return ;
	}

	//�ж�Բ�İ뾶�Ƿ��ʺ���offset
	double dblRds = pCircle->radius();
	double ofstVal = m_offsetDist / 2.0;
	if((dblRds + ofstVal) <= TOL_CCW)
	{
		addLayer(_T("problem"));
		pCircle->upgradeOpen();
		pCircle->setLayer(_T("problem"));  
		pCircle->downgradeOpen();  
		return ;
	}
	else
	{
		pCircle->upgradeOpen();
		pCircle->setRadius(dblRds + ofstVal);  
		pCircle->downgradeOpen();  
		//pCircle->close();  //here��don't close it;   
		return ;
	}
}


//����Ƿ�Ϊ������,����Ƕ������Ƿ�պ�,����?
bool
CPlineOffset::checkPolyline(IN const AcDbEntity* pEnt)
{
	if(pEnt->isA() != AcDbPolyline::desc())
	{
		return false;
	}

	//is closed
	AcDbPolyline* pPline = (AcDbPolyline*)pEnt;
	if(!(pPline->isClosed()))
	{
		return false;
	}

	//is zero width? ��Ҫ��������?
	return true;  
}


//���offset�����Ƿ���ȷ��ͨ�����offset���ͼ��������С����ԭͼ��������С���Ա����ߣ�
//true ���� ��ȷok;
bool
CPlineOffset::checkOffsetRsltSize(IN const AcDbExtents orgExt,IN const AcDbExtents newExt,IN const double dblOfstSize)
{
	bool bFlag = false;
	if(CExtentOfSS::isExtEffect(orgExt) == false)
	{
		return false;
	}

	if(CExtentOfSS::isExtEffect(newExt) == false)
	{
		return false;
	}

	//�ж��Ƿ�ͬʱΪ������ͬʱΪ��; 
	double xDelt = newExt.maxPoint().x - orgExt.maxPoint().x;  
	if((xDelt > 0 && dblOfstSize > 0) ||(xDelt < 0 && dblOfstSize <0))
	{
		return true;
	}
	else  //�������һ���д�;  
	{
		return false;
	}
}


//��ѡ���еĶ�����ִ��offset;
void
CPlineOffset::ofst(IN ads_name ss,OUT AcDbVoidPtrArray& m_plineArr)
{
	if(m_offsetDist == 0)  //ƫ��ֵ����0��û������;
	{
		return;
	}

	AcDbEntity* pEnt;
	AcDbPolyline* pPline;
	AcDbObjectId id;
	ads_name ssUnit;
	//open the entity
	long nLen = 0;
	acedSSLength(ss,&nLen);
	if(nLen <= 0)
	{
		return;
	}

	for(long i = 0; i < nLen; i++)
	{
		acedSSName(ss,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenObject(pEnt,id,AcDb::kForRead);
		if(pEnt->isA() == AcDbPolyline::desc())
		{
			pPline = (AcDbPolyline*)pEnt;  
			ofst(pPline,m_plineArr);	
		}	
		else if(pEnt->isA() == AcDbCircle::desc())
		{
			AcDbCircle* pCircle = (AcDbCircle*)pEnt;
			ofst(pCircle);
		}
		else
		{
			addLayer(_T("problem"));
			pEnt->upgradeOpen();
			pEnt->setLayer(_T("problem"));  
			pEnt->downgradeOpen();  
			
		}
		pEnt->close(); 
	}
	return;
}


//�պ϶�����ƫ�������Ӧ�ĺ���
void
ofstPline()
{
	ads_name ss;
	select(ss);

	ads_real dblOfstDist = 0;  //offsetƫ��ֵ;
	int iFlag;
	do 
	{
		iFlag =  acedGetReal(_T("\nPlease input the offset value:"),&dblOfstDist);
		if(iFlag == RTCAN)
		{
			acutPrintf(_T("\nUser canceled."));
			acedSSFree(ss);
			return;
		}		
	} while (iFlag != RTNORM);
	 

	AcDbVoidPtrArray plinePtrArr;
	CPlineOffset objOffsetPl;
	objOffsetPl.setOffsetDist(dblOfstDist);
	objOffsetPl.ofst(ss,plinePtrArr);

	//�����߼������ݿ�;
	AcDbPolyline* pPline;
	void* ptrVoid = NULL;
	int nNum = 0;
	nNum = plinePtrArr.length();
	for(int i = 0; i < nNum; i++) 
	{
		ptrVoid = plinePtrArr.at(i);  
		pPline = (AcDbPolyline*)ptrVoid;  
		join2database(pPline);
		pPline->close();
	}

	//release
	acedSSFree(ss);
}


//===================
//class CIsPlineCCW
//===================
CIsPlineCCW::CIsPlineCCW():m_tol(VECTOR_TOL)
{
	m_isClosed = NOT_CLOSED;
	m_nNumVerts = 0;
}


CIsPlineCCW::~CIsPlineCCW()
{
}


//constructor
CIsPlineCCW::CIsPlineCCW(IN  AcDbPolyline* pPline):m_tol(VECTOR_TOL)
{
	if(pPline != NULL)
	{
		m_pline = pPline;
		m_isClosed =isClosed(pPline);
		m_nNumVerts = pPline->numVerts();
	}
	m_allVec3d.clear();
	m_crossProductNum.clear();
}


//input the polyline
//������ָ��Ϊ�գ��򷵻�false;
bool
CIsPlineCCW::inputPolyline(IN  AcDbPolyline* pPline)
{
	m_allVec3d.clear();

	m_crossProductNum.clear();

	if(pPline != NULL)
	{
		m_isClosed = isClosed(pPline);
		m_pline = pPline;
		m_nNumVerts = pPline->numVerts();
		return true;
	}
	return false;
}


//is polyline closed;
//��������߲��պϣ�����������յ��Ƿ��غ�.
EIsPlineClosed
CIsPlineCCW::isClosed(IN const AcDbPolyline* pline)
{
	if(pline->isClosed())
	{
		return REAL_CLOSED;
	}

	AcGePoint3d ptStart;
	AcGePoint3d ptEnd;
	int nNum = pline->numVerts();
	if(nNum <= 2)
	{
		return NOT_CLOSED;
	}

	AcGeTol objTol;
	objTol.setEqualPoint(0.000001); //���ֵ��Ϊ6λС��;
	pline->getStartPoint(ptStart);
	pline->getEndPoint(ptEnd);
	if(ptStart.isEqualTo(ptEnd,objTol))
	{
		return ASSUME_CLOSED;
	}
	return NOT_CLOSED;
}



//�������ĳ�����㴦��vector3d
//��ÿ�������2��vector��һ����㴦��һ���յ㴦;
//���ֻ��Ҫ����vector��нǣ��Ϳ������ÿ�����㴦�ļн�;
//��Ϊ����������мнǵ��㷨;
void 
CIsPlineCCW::calVectorOfVertex(IN const int iIndex)
{
	AcGeVector3d kxVec(1,0,0);  //����x��

	AcGeVector3d vect3d;
	AcGePoint3d ptStart;
	AcGePoint3d ptEnd;
	AcGePoint3d ptCenter;
	
	int iIndex1 = iIndex; //��һ������;
	int iIndex2 = (iIndex1 + 1) % m_nNumVerts; //�ڶ�������;

	//���һ������,�˳�;
	if(iIndex1 == m_nNumVerts)
	{
		return;
	}

	m_pline->getPointAt(iIndex1,ptStart);
	m_pline->getPointAt(iIndex2,ptEnd);

	if(m_pline->segType(iIndex1) == AcDbPolyline::kLine)
	{
		double x = ptStart.x - ptEnd.x;
		double y = ptStart.y - ptEnd.y;
		double z = ptStart.z - ptEnd.z;
		vect3d.set(x,y,z);
		m_allVec3d.push_back(vect3d);  //�õ��߶ε�����;
		m_allVec3d.push_back(vect3d);  //���߶�ҲҪ��2��
	}
	else if(m_pline->segType(iIndex1) == AcDbPolyline::kArc)
	{
		//�󻡶�����㴦��vector;
		double dblBugle = 0;
		m_pline->getBulgeAt(iIndex1,dblBugle);
		AcGeCircArc3d segArc3d;
		m_pline->getArcSegAt(iIndex1,segArc3d);
		ptCenter =  segArc3d.center();
		AcGeVector3d  vec1;
		AcGeVector3d vec2;
		
		//�������ĵ���ʼ�˵������;
		double x = ptStart.x - ptCenter.x;
		double y = ptStart.y - ptCenter.y;
		double z = ptStart.z - ptCenter.z;
		vec1.set(x,y,z);

		//�󻡶����յ㴦��vector
		x = ptEnd.x - ptCenter.x;
		y = ptEnd.y - ptCenter.y;
		z = ptEnd.z - ptCenter.z;
		vec2.set(x,y,z);
		//������������ת����ö˵������;
		//����bugle�����жϻ�������ʱ�뻹��˳ʱ�룬�Ӷ����������ж�Ӧ��ת;
		if(dblBugle > 0)
		{
			vec1.rotateBy(Pi/4,kxVec);
			vec2.rotateBy(Pi/4,kxVec);
		}
		else
		{
			vec1.rotateBy(-Pi/4,kxVec);
			vec2.rotateBy(Pi/4,kxVec);
		}
		m_allVec3d.push_back(vec1);
		m_allVec3d.push_back(vec2);
	}
}


//��ȡ���ж����vector;
//���һ�����㲻���㣬��Ϊ����ǰһ�������Ѿ��������;
void
CIsPlineCCW::getAllVecOfVtx()
{
	for(int i = 0; i < m_nNumVerts; i++) //���һ��������Լ��㣬��ʹ������Ч����;
	{
		calVectorOfVertex(i);
	}

	//�������vector�󣬽����һ��vector�ƶ�����ǰ��;�Ա��������ȡ��vector����н�;
	int nNum = m_allVec3d.size();
	if(nNum <= 0)
	{
		return;
	}

	AcGeVector3d vecLast = m_allVec3d.back();
	vector<AcGeVector3d>::iterator itr = m_allVec3d.begin();
	m_allVec3d.insert(itr,1,vecLast); //insert
	itr = m_allVec3d.end();
	itr--;
	m_allVec3d.erase(itr);  //erease;
}


//����vector3d���ϣ���ȡÿ�����㴦�н�;
//�н��Ե�һ���߿�ʼ���ڶ����߽������ߵ�˳�����ڶ������е�˳��Ϊ׼;
//�����յ�Ҫ�ر���;
void
CIsPlineCCW::getAllAngles()
{
	getFirstAng(); //��һ���������⴦��;
	
	//������������ĽǶ�;
	double dblCrossProductNum = 0;
	int iIndexStart;
	int iIndexEnd;
	AcGeVector3d vec1;
	AcGeVector3d vec2;
	AcGeVector3d vec3;
	int nNum = m_allVec3d.size();
	for(int i = 2; i+1 < nNum; i+=2 ) //0��1�ǵ�һ�����㣻��2��ʼ;
	{
		iIndexStart = i;
		iIndexEnd = i+1;
		vec1 = m_allVec3d[iIndexStart];
		vec2 = m_allVec3d[iIndexEnd];
		vec3 = vec1.crossProduct(vec2);
		 dblCrossProductNum = vec3.z;  //z�����ֵ������������Ŵ���vec1��vec2��ת����ʱ�뻹��˳ʱ��;
		if(abs(dblCrossProductNum) > m_tol)  //����0�ĽǶȲ��ռ�;
		{
			m_crossProductNum.push_back(dblCrossProductNum);
		}
	}
}


//�����һ������ļн�,��һ������ļн�Ҫ���⴦��;
//����ϵ���Ѿ��������,ֱ������ȡ��������нǼ���;
void
CIsPlineCCW::getFirstAng()
{
	const int nNumOfVec = m_allVec3d.size();  //

	double dblCrossProductNum = 0;
	int iIndexFirst = 0;
	int iIndexSecond = 1;
	
	AcGeVector3d vec1 = m_allVec3d[iIndexFirst];
	AcGeVector3d vec2 = m_allVec3d[iIndexSecond];	
	
	if(m_isClosed != NOT_CLOSED)  //�����߲��պ�;
	{
		dblCrossProductNum = 0;
		return;
		//m_allAngels.push_back(dblAng);
	}
	else  //�պ������������һ������;
	{
		AcGeVector3d vec3 = vec1.crossProduct(vec2);
		dblCrossProductNum = vec3.z;
		//m_allAngels.push_back(dblAng);s
	}

	//����dblAng;  �Ƕ�Ϊ0���ռ�.
	if(abs(dblCrossProductNum) <= m_tol)
	{
		dblCrossProductNum = 0;
		return;
	}
	m_crossProductNum.push_back(dblCrossProductNum);
}


//��������ʱ���ж�
//ͨ�������߶��㴦���������ߣ��������������z����ֵ���������������ж��Ƿ���ʱ��;
//���нǵĸ������ฺ�нǵĸ�������Ϊ��ʱ��;
//����ȥ�������ظ�����ֵ�����߸�ֵ,ֻ����һ��;
//�������һ����������Ҫͬ��һ�����������������ǣ������ͬ��ҲҪ������Ӧ����;
EIsPlineCCW
CIsPlineCCW::isCCW()
{
	int nNum = m_crossProductNum.size();
	if(nNum <= 0)
	{
		return ERROR_PLINE;
	}

	int nNumPos = 0; //���Ƕȵĸ���;
	int nNumNeg = 0; //���Ƕȵĸ���;
	bool bPre = true; //ǰһ������״̬:����;
	bool bNow = true;
	double dblAng = 0;
	//��һ������״̬;
	dblAng = m_crossProductNum.front();
	bPre = dblAng > 0? true:false;
	bPre == true? nNumPos++:nNumNeg++;
	//��������״̬��ͳ��;
	vector<double>::iterator itr = m_crossProductNum.begin();
	itr++;
	for(; itr != m_crossProductNum.end(); itr++)
	{
		dblAng = (double)(*itr);
		bNow = dblAng >0? true:false;
		if(bNow == bPre)
		{
			continue;
		}
		else
		{
			dblAng >0? nNumPos++:nNumNeg++;
			bPre = bNow;
		}
	}

	//�ж�
	return (nNumPos > nNumNeg? UNCLOCKWISE_PLINE:CLOCKWISE_PLINE);

	//�����������������ϣ�����Ҫ��
	//��һ���������һ����Ҫ�Ƚ�״�壬�����ͬ��������Ӧ����Ҫ��ȥһ;
	//double firstAng = m_allAngles.front();
	//double secondAng = m_allAngles.back();
	//bool bFirst = true;
	//bool bSecond = true;
	//bFirst = firstAng > 0? true:false;
	//bSecond = secondAng > 0? true:false;
	//if(bFirst == bSecond)
	//{
	//	if(bFirst == true)
	//	{
	//		nNumPos--;
	//	}
	//	else
	//	{
	//		nNumNeg--;
	//	}
	//}

	//if(nNumNeg > nNumPos) //�������
	//{
	//	return false;
	//}
	//else
	//{
	//	return true;  //��ʱ��;
	//}
}


//�ж϶������Ƿ���ʱ��;
EIsPlineCCW
CIsPlineCCW::isPlineCCW(IN AcDbPolyline* pPline)
{	
	if(!inputPolyline(pPline))
	{
		return ERROR_PLINE; //������������;
	}

	//ȡvector����
	getAllVecOfVtx();

	//ȡ�нǼ���;
	getAllAngles();

	EIsPlineCCW bIsCCW = ERROR_PLINE; //init
	bIsCCW = isCCW();

	return bIsCCW;
}


//=============================
//class CJudgePlineCCW 
//=============================
CJudgePlineCCW::CJudgePlineCCW(IN AcDbPolyline* pPline)
{
	m_pline = NULL;
	inputPline(pPline);
}


//deconstructor
CJudgePlineCCW::CJudgePlineCCW()
{
}


//deconstructor
CJudgePlineCCW::~CJudgePlineCCW()
{
}


//input polyline pointer
bool
CJudgePlineCCW::inputPline(IN AcDbPolyline* pPline)
{
	if(pPline == NULL)
	{
		 return false;
	}

	m_pline = pPline;
	m_nNumVerts = m_pline->numVerts();
	if(m_nNumVerts <= 1)
	{
		return false;
	}

	return true;
}


//ȡĳ���ߵ���ߵ�״̬;
//bool
//CJudgePlineCCW::prsTopY(IN const int iIndexVtx,OUT struct STopY& sta)
//{
//	STopY st;
//	return st;
//}

//�ж϶������Ƿ���ʱ��;
//������������;
EIsPlineCCW 
CJudgePlineCCW::isPlineCCW()
{
	//�ж϶������Ƿ����
	if(m_pline == NULL)
	{
		return ERROR_PLINE;
	}

	STopY sta;
	if(!prsTopY(sta))
	{
		return ERROR_PLINE;
	}

	//
	EIsPlineCCW rslt;
	switch(sta.m_topYLocation)
	{
	case TOPY_ALL_LINE:
		rslt = whenAllLine(sta);
		return rslt;
	case TOPY_IN_MIDDLE:
		rslt = whenPtOnMiddle(sta);
		return rslt;
	case TOPY_ON_END:
		rslt = whenPtOnEnd(sta);
		return rslt;
	case TOPY_ON_START:
		rslt = whenPtOnStart(sta);
		return rslt;
	default:
		return ERROR_PLINE;
	}

	return ERROR_PLINE;
}


//������lineʱ,ȡ��ߵ�״̬;
//struct  STopY
//CJudgePlineCCW::prsTopYLineSeg(IN const int iIndexVtx)
//{
//	struct STopY sta;
//
//	AcGeLineSeg2d objLine2d;
//	m_pline->getLineSegAt(iIndexVtx,objLine2d);
//	AcGePoint3d ptStart;
//	AcGePoint3d ptEnd;
//	ptStart = objLine2d.startPoint();
//	ptEnd = objLine2d.endPoint();
//
//	if(ptStart.y >= ptEnd.y)
//	{
//		sta.m_iIndexVtx = iIndexVtx;
//		sta.m_pt2d = ptStart;
//		sta.m_topYLocation = TOPY_ON_START;
//		sta.m_y = ptStart.y;
//		return sta;
//	}
//	else
//	{
//		sta.m_iIndexVtx = iIndexVtx ;
//		sta.m_pt2d = ptStart;
//		sta.m_topYLocation = TOPY_ON_END;
//		sta.m_y = ptStart.y;
//		return sta;
//	}
//};


////������arcʱ,ȡ��ߵ�״̬;    //old version,give up;
//bool
//CJudgePlineCCW::prsTopYArcSeg(IN const int iIndexVtx,OUT struct STopY& sta)
//{
//	if(m_pline->segType(iIndexVtx) != AcDbPolyline::kArc)
//	{
//		return false;
//	}
//
//	//��bugle,�����жϻ�������ʱ�뻹��˳ʱ��;
//	double dblBugle = 0;
//	m_pline->getBulgeAt(iIndexVtx,dblBugle);  
//
//	AcGeCircArc2d carc2d;
//	m_pline->getArcSegAt(iIndexVtx,carc2d);  
//	
//
//	AcGePoint2d ptStart = carc2d.startPoint();
//	AcGePoint2d ptEnd = carc2d.endPoint();
//	AcGePoint2d ptCenter = carc2d.center();  
//	double dblRadiu = carc2d.radius();
//	AcGeVector2d vec1(ptStart.x - ptCenter.x, ptStart.y - ptCenter.y);
//	AcGeVector2d vec2(ptEnd.x - ptCenter.x, ptEnd.y - ptCenter.y);
//	//�ж���ߵ��ǲ����ڻ��м�;
//	double angleStart = vec1.angle();    
//	double angleEnd = vec2.angle();   
//	double angleMid = Pi / 2.0;  
//	bool isMidHigher = false;
//	if((angleEnd - angleStart) > TOL_CCW)  //����
//	{
//		if((angleMid - angleStart) > TOL_CCW && (angleEnd - angleMid) < TOL_CCW) 
//		{
//			isMidHigher = true;
//		}
//	}
//	else if((angleEnd - angleStart) < TOL_CCW)  //��ʼ�Ƕȴ����յ�Ƕ�;˵���յ�Ƕȴ���360��;
//	{
//		if((angleMid - angleStart) > TOL_CCW || (angleEnd - angleMid) < TOL_CCW) 
//		{
//			isMidHigher = true;  
//		}
//	}
//	else  //���ᷢ��;
//	{
//		acutPrintf(_T("\nThis arc is zero degree! error arc..."));  
//		isMidHigher = true;
//	}
//
//	//�ж���ߵ�;
//	if(isMidHigher == true)
//	{
//		sta.m_iIndexVtx = iIndexVtx;
//		sta.m_pt2d.set(ptCenter.x,ptCenter.y+dblRadiu); 
//		sta.m_topYLocation = TOPY_IN_MIDDLE;
//		sta.m_y = ptCenter.y + dblRadiu;
//		return true;
//	}
//	else  //�жϻ��Ķ˵�;  
//	{
//		//���������
//		vec1.set(dblRadiu,0); 
//		vec1.rotateBy(angleStart);
//		ptStart.set(ptCenter.x + vec1.x, ptCenter.y+ vec1.y);
//
//		vec1.set(dblRadiu,0);
//		vec1.rotateBy(angleEnd);
//		ptEnd.set(ptCenter.x + vec1.x, ptCenter.y+vec1.y);
//
//		if((ptStart.y - ptEnd.y) >= TOL_CCW) //���߻��ߵȸ�;
//		{
//			sta.m_iIndexVtx = iIndexVtx;
//			sta.m_pt2d = ptStart;
//			sta.m_topYLocation = (dblBugle > 0)? TOPY_ON_START:TOPY_ON_END; //AcGeCircArc2d���������"�����߻���"��һ����ͬ;
//			sta.m_y = ptStart.y;
//			return true;
//		}
//		else //if((ptEnd.y - ptStart.y ) > TOL_CCW)
//		{
//			sta.m_iIndexVtx = iIndexVtx;
//			sta.m_pt2d = ptEnd;
//			sta.m_topYLocation = (dblBugle > 0)?TOPY_ON_END:TOPY_ON_START; //AcGeCircArc2d���������"�����߻���"��һ����ͬ;
//			sta.m_y = ptEnd.y;
//			return true;
//		}		
//	}
//	return true;
//};


//������arcʱ,ȡ��ߵ�״̬;
//���ж���ߵ��Ƿ�Ͷ˵��غϣ� ���ж���ߵ��Ƿ��ڻ����м䣻
//Ȼ���ж϶˵��Ƿ���ߵ�;
bool
CJudgePlineCCW::prsTopYArcSeg(IN const int iIndexVtx,OUT struct STopY& sta)
{
	if(m_pline->segType(iIndexVtx) != AcDbPolyline::kArc)
	{
		return false;
	}

	AcGeCircArc2d carc2d;
	m_pline->getArcSegAt(iIndexVtx,carc2d);  

	double dblRadiu = carc2d.radius();
	AcGePoint2d ptStart = carc2d.startPoint();
	AcGePoint2d ptEnd = carc2d.endPoint();
	AcGePoint2d ptCenter = carc2d.center();  
	AcGePoint2d ptHighst(ptCenter.x,ptCenter.y + dblRadiu); //��ߵ�����;
	
	AcGeTol objTol;
	objTol.setEqualPoint(TOL_CCW);
	//�ж���ߵ��Ƿ�Ͷ˵��غ�;
	if(ptHighst.isEqualTo(ptStart,objTol))
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptStart;
		sta.m_topYLocation = TOPY_ON_START; //AcGeCircArc2d���������"�����߻���"��һ����ͬ;
		sta.m_y = ptStart.y;
		return true;
	}
	else if(ptHighst.isEqualTo(ptEnd,objTol))
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptEnd;
		sta.m_topYLocation = TOPY_ON_END; //AcGeCircArc2d���������"�����߻���"��һ����ͬ;
		sta.m_y = ptEnd.y;
		return true;
	}

	//�ж���ߵ��Ƿ��ڻ�����; �Ҳ���˵��غ�; ����غϣ��������Ѿ�������;
	Adesk::Boolean isHigstPtOnArc = false;	
	isHigstPtOnArc = carc2d.isOn(ptHighst,objTol);
	if(isHigstPtOnArc == kTrue)
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptHighst;
		sta.m_topYLocation = TOPY_IN_MIDDLE; //AcGeCircArc2d���������"�����߻���"��һ����ͬ;
		sta.m_y = ptHighst.y;
		return true;
	}	

	//��ߵ㲻�ڻ����ϣ���˵��е�һ��Ϊ��ߵ�;
	if((ptStart.y - ptEnd.y) >= TOL_CCW) //���߻��ߵȸ�;
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptStart;
		sta.m_topYLocation = TOPY_ON_START; //AcGeCircArc2d���������"�����߻���"��һ����ͬ;
		sta.m_y = ptStart.y;
		return true;
	}
	else //if((ptEnd.y - ptStart.y ) > TOL_CCW)
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptEnd;
		sta.m_topYLocation = TOPY_ON_END; //AcGeCircArc2d���������"�����߻���"��һ����ͬ;
		sta.m_y = ptEnd.y;
		return true;
	}	
}


//�жϻ�����ߵ��ǲ����ڻ����м䣬���Ƕ˵�;
//bool
//CJudgePlineCCW::isHigherPtOn(IN const double startAng,IN const double endAng,IN const double dblBug)
//{
//	;
//}


//��ֱ�߶ε���ߵ�
//����ֵ��true ��Ч�� false����Ч;
bool
CJudgePlineCCW::prsTopYLineSeg(IN const int iIndexVtx,OUT struct STopY& sta)
{
	if(m_pline->segType(iIndexVtx) != AcDbPolyline::kLine)
	{
		return false;
	}

	AcGeLineSeg2d lnSeg2d;
	m_pline->getLineSegAt(iIndexVtx,lnSeg2d);
	AcGePoint2d ptStart = lnSeg2d.startPoint();
	AcGePoint2d ptEnd = lnSeg2d.endPoint();
	//�ж�yֵ;
	if((ptStart.y - ptEnd.y) > TOL_CCW) 
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptStart;
		sta.m_topYLocation = TOPY_ON_START;
		sta.m_y = ptStart.y;
	}
	else if((ptEnd.y - ptStart.y ) > TOL_CCW)
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptStart;
		sta.m_topYLocation = TOPY_ON_END;
		sta.m_y = ptStart.y;
	}
	else
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptStart;
		sta.m_topYLocation = TOPY_ALL_LINE;
		sta.m_y = ptStart.y;
	}
	return true;
}


//��ȡ��ߵ�
bool
CJudgePlineCCW::prsTopY(OUT struct STopY& sta)
{
	AcGeLineSeg2d geLine2d;
	AcGeCircArc2d geArc2d;

	int nNumVerts = m_pline->numVerts();  

	//��ȡ��һ��sta;
	struct STopY staTemp;	
	int j = 0;
	for(; j < nNumVerts; j++)
	{
		if(m_pline->segType(j) == AcDbPolyline::kLine)
		{
			prsTopYLineSeg(j,staTemp);
			break;
		}
		else if(m_pline->segType(j) == AcDbPolyline::kArc)
		{
			prsTopYArcSeg(j,staTemp);
			break;
		}
		else
		{
			continue;
		}
	}

	//���˵�һ��sta�󣬿��Խ��к����Ƚ���;
	struct STopY staNow;
	for(int i = j; i < nNumVerts; i++)
	{
		if(m_pline->segType(i) == AcDbPolyline::kLine)
		{
			prsTopYLineSeg(i,staNow);
			if((staTemp.m_y - staNow.m_y) < TOL_CCW) //�Ƚ�y����;  
			{
				staTemp = staNow;
			}
		}
		else if(m_pline->segType(i) == AcDbPolyline::kArc)
		{
			prsTopYArcSeg(i,staNow);
			if((staTemp.m_y - staNow.m_y) < TOL_CCW) //�Ƚ�y����
			{
				staTemp = staNow;
			}
		}
		else
		{
			continue;
		}
	}
	sta = staTemp;
	return true;
}


//������߶���һ��ˮƽ�ߵ�ʱ��
//���ݸ�ˮƽ���ж�˳��ʱ��;
//�Ƕ���0����ʾ˳ʱ�룻�Ƕ���pi����ʾ��ʱ��;  
EIsPlineCCW
CJudgePlineCCW::whenAllLine(const STopY& sta)
{
	if(sta.m_topYLocation != TOPY_ALL_LINE)
	{
		return ERROR_PLINE;
	}

	//���߶�Ϊˮƽ��;
	AcGeLineSeg2d lnseg2da;
	AcGePoint2d pt1;
	AcGePoint2d pt2;
	AcGeLineSeg2d lnseg2d;
	m_pline->getLineSegAt(sta.m_iIndexVtx,lnseg2d);
	pt1 = lnseg2d.startPoint();
	pt2 = lnseg2d.endPoint();
	if(pt1.x > pt2.x)  //180��
	{
		return UNCLOCKWISE_PLINE;  //˳ʱ��;
	}
	else if(pt1.x < pt2.x)
	{
		return CLOCKWISE_PLINE; //��ʱ��; 
	}
	else   //��pt1.x == pt2.x��˵��������������(û�������);
	{ 
		return ERROR_PLINE;
	}	
}


//��ߵ��ڻ����ϣ��Ƕ˵�;
//�жϻ���˳��ʱ�뼴��; ��������������ߵ�����;
EIsPlineCCW
CJudgePlineCCW::whenPtOnMiddle(const STopY& sta)
{
	//�ж��Ƿ�
	if(sta.m_topYLocation != TOPY_IN_MIDDLE)
	{
		return ERROR_PLINE;
	}

	int iIndex = sta.m_iIndexVtx;
	//�жϴ˻�������;
	AcGeCircArc2d arc2da;
	double dblBugle = 0;
	m_pline->getBulgeAt(iIndex,dblBugle);
	return (dblBugle >0)? UNCLOCKWISE_PLINE:CLOCKWISE_PLINE;
}


//��ߵ���line�����;
//����ȡ��һ�����Σ��ټ����������εĽǶȣ��Ƚ������Ƕȣ��ó�����������;
//���أ��Ƿ���ʱ��;
//bCheckType�����sta�Ƿ�TOPY_ON_START
EIsPlineCCW
CJudgePlineCCW::whenPtOnStart(const STopY& sta)  
{
	//check
	if(sta.m_topYLocation != TOPY_ON_START)  
	{
		return ERROR_PLINE; 
	}

	//�����һ���߶���Ϣ; 
	//�������Ƕ�; 
	int numTotalVtxs = 0;  
	numTotalVtxs =  m_pline->numVerts();  
	STopY stb; 
	stb.m_iIndexVtx = (sta.m_iIndexVtx - 1 + numTotalVtxs) % numTotalVtxs;  //��ֹ�õ�����;   
	stb.m_pt2d = sta.m_pt2d;
	stb.m_topYLocation = TOPY_ON_END;  
	stb.m_y = sta.m_y;

	double anglea = -1;
	double angleb  = -1;
	anglea = calRadian(stb);  
	angleb = calRadian(sta);  
	//�Ƕ�ֵ��һ���������ܵ���0(�޸�Ϊ2Pi);  
	anglea = anglea < TOL_CCW ? 2*Pi:anglea; 
	angleb = angleb < TOL_CCW ? 2*Pi:angleb; 
	if(angleb > anglea)
	{
		return CLOCKWISE_PLINE; //˳ʱ��   
	}
	else if(angleb < anglea)   
	{
		return UNCLOCKWISE_PLINE;   
	}

	return ERROR_PLINE;  //
}



//��ߵ���line�����;
//����ȡ��һ�����Σ��ټ����������εĽǶȣ��Ƚ������Ƕȣ��ó�����������;
//���أ��Ƿ���ʱ��;
//bCheckType�����sta�Ƿ�TOPY_ON_START
EIsPlineCCW
CJudgePlineCCW::whenPtOnEnd(const STopY& sta)
{
	//check
	if(sta.m_topYLocation != TOPY_ON_END)
	{
		return ERROR_PLINE; 
	}	

	//�����һ���߶���Ϣ; 
	//�������Ƕ�; 
	int numTotalVtxs = 0;
	numTotalVtxs = m_pline->numVerts();
	STopY stb; 
	stb.m_iIndexVtx = (sta.m_iIndexVtx + 1) % numTotalVtxs;  //��ֹ������Χ;
	stb.m_pt2d = sta.m_pt2d; 
	stb.m_topYLocation = TOPY_ON_START;  
	stb.m_y = sta.m_y;

	double anglea = -1;
	double angleb  = -1;
	anglea = calRadian(sta);
	angleb = calRadian(stb);
	//�Ƕ�ֵ��һ���������ܵ���0(�޸�Ϊ2Pi);  
	anglea = anglea < TOL_CCW ? 2*Pi:anglea; 
	angleb = angleb < TOL_CCW ? 2*Pi:angleb; 
	if(angleb - anglea > TOL_CCW)  //����;
	{
		return CLOCKWISE_PLINE; //˳ʱ��
	}
	else if(angleb - anglea < (0 - TOL_CCW))
	{
		return UNCLOCKWISE_PLINE;
	}

	return ERROR_PLINE;  //
}


//������ߵ��������û��νǶ�;
//�����TOPY_IN_MIDDLE���;���������������;
//ע�����ֻ�����;
//���ؽǶ�ֵ�� ����-1��ʾʧ��;
double 
CJudgePlineCCW::calRadian(const STopY& sta)
{
	AcGeVector2d vec2dv;
	AcGePoint3d pt1;
	AcGePoint3d pt2;
	AcGeLineSeg3d lnseg3da;
	double anglea = -1;
	//
	if(m_pline->segType(sta.m_iIndexVtx) == AcDbPolyline::kLine)
	{
		if(sta.m_topYLocation == TOPY_ON_START)  //line and start point
		{
			m_pline->getLineSegAt(sta.m_iIndexVtx,lnseg3da);  
			pt1 = lnseg3da.startPoint();
			pt2 = lnseg3da.endPoint();
			vec2dv.set(pt2.x - pt1.x, pt2.y - pt1.y);
			anglea = vec2dv.angle();
			return anglea;
		}
		else if(sta.m_topYLocation == TOPY_ON_END)
		{
			m_pline->getLineSegAt(sta.m_iIndexVtx,lnseg3da);
			pt1 = lnseg3da.startPoint();
			pt2 = lnseg3da.endPoint();
			vec2dv.set(pt1.x - pt2.x, pt1.y - pt2.y);
			anglea = vec2dv.angle();
			return anglea;
		}
		else if(sta.m_topYLocation == TOPY_ALL_LINE)
		{
			m_pline->getLineSegAt(sta.m_iIndexVtx,lnseg3da);
			pt1 = lnseg3da.startPoint();
			pt2 = lnseg3da.endPoint();
			vec2dv.set(pt2.x - pt1.x, pt2.y - pt1.y);
			anglea = vec2dv.angle();
			return anglea;
		}
	}
	else if(m_pline->segType(sta.m_iIndexVtx) == AcDbPolyline::kArc)
	{
		AcGeCircArc2d arc2da;
		AcGeVector2d vec2da;
		AcGePoint2d pt1;
		AcGePoint2d pt2;
		double anglea;
		double dblBug = 0;
		m_pline->getBulgeAt(sta.m_iIndexVtx,dblBug);  //�жϻ�������ʱ�뻹��˳ʱ��.
		if(sta.m_topYLocation == TOPY_ON_START)  //line and start point
		{
			m_pline->getArcSegAt(sta.m_iIndexVtx,arc2da);  
			pt1 = arc2da.startPoint();
			pt2 = arc2da.center();
			vec2da.set(pt1.x - pt2.x, pt1.y - pt2.y);
			(dblBug > 0)?(vec2da.rotateBy(Pi*0.5)):(vec2da.rotateBy(-Pi*0.5));			
			anglea = vec2da.angle();
			return anglea;
		}
		else if(sta.m_topYLocation == TOPY_ON_END)
		{
			m_pline->getArcSegAt(sta.m_iIndexVtx,arc2da);
			pt1 = arc2da.endPoint();
			pt2 = arc2da.center();
			vec2da.set(pt1.x - pt2.x, pt1.y - pt2.y);
			(dblBug > 0)?(vec2da.rotateBy(-Pi*0.5)):(vec2da.rotateBy(Pi*0.5));	
			anglea = vec2da.angle();
			return anglea;
		}
	}

	return -1;  //�Ƕ���û�и����ģ�����-1��ʾʧ��;
}


//=============================
//class CIsPlineCCW test function
//=============================
//������CIsPlineCCW�����Զ���������ʱ���
void 
testIfCCWPline()
{
	ads_name ssUnit;
	ads_name ss;
	select(ss);

	long nlen = 0;
	acedSSLength(ss,&nlen);
	
	AcDbObjectId id;
	AcDbEntity* pEnt;
	AcDbPolyline* pPline;
	CIsPlineCCW objIsPlnCCW;
	for(long i = 0; i < nlen; i++)
	{
		acedSSName(ss,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);
		if(pEnt->isA() == AcDbPolyline::desc())
		{
			pPline = (AcDbPolyline*)pEnt;
			EIsPlineCCW es = objIsPlnCCW.isPlineCCW(pPline);
			switch (es)
			{
			case UNCLOCKWISE_PLINE:
				acutPrintf(_T("\nUNCLOCKWISE_PLINE"));
				break;
			case CLOCKWISE_PLINE:
				acutPrintf(_T("\nCLOCKWISE_PLINE"));
				break;
			case ERROR_PLINE:
				acutPrintf(_T("\nERROW_PLINE"));
				break;	
			}
		}
		pEnt->close();
	}

	acedSSFree(ss);
}



//=============================
//class CJudgePlineCCW test function
//=============================
//������CJudgePlineCCW�����Զ���������ʱ���
void 
testPlineCCW()
{
	ads_name ssUnit;
	ads_name ss;
	select(ss);

	long nlen = 0;
	acedSSLength(ss,&nlen);

	AcDbObjectId id;
	AcDbEntity* pEnt;
	AcDbPolyline* pPline;
	CJudgePlineCCW objIsPlnCCW;
	for(long i = 0; i < nlen; i++)
	{
		acedSSName(ss,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);
		if(pEnt->isA() == AcDbPolyline::desc())
		{
			pPline = (AcDbPolyline*)pEnt;
			objIsPlnCCW.inputPline(pPline);
			EIsPlineCCW es = objIsPlnCCW.isPlineCCW();
			switch (es)
			{
			case UNCLOCKWISE_PLINE:
				acutPrintf(_T("\nUNCLOCKWISE_PLINE"));
				break;
			case CLOCKWISE_PLINE:
				acutPrintf(_T("\nCLOCKWISE_PLINE"));
				break;
			case ERROR_PLINE:
				acutPrintf(_T("\nERROW_PLINE"));
				break;	
			}
		}
		pEnt->close();
	}

	acedSSFree(ss);
}