/************************************************************
Copyright (C), 2016-6-12, XuMiJieZi procision maskmaking. Co., Ltd.
FileName: ofstAndChamfer.cpp
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


//对某多义线执行offset
void
CPlineOffset::ofst(IN AcDbPolyline* pPlineOrg,OUT AcDbVoidPtrArray& m_plineArr)
{
	if(m_offsetDist == 0)
	{
		return;  
	}

	if(!checkPolyline(pPlineOrg)) //多义线不合格，放入问题层;    
	{
		addLayer(_T("problem"));  
		pPlineOrg->setLayer(_T("problem"));  
		return;
	}

	//取extent;
	AcDbExtents orgExt;
	AcDbExtents newExt;
	pPlineOrg->getGeomExtents(orgExt);

	//判断多义线是否逆时针;
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
		//offset失败，将该实体放入“problem”层;  
		addLayer(_T("problem"));
		pPlineOrg->setLayer(_T("problem"));
		return;
	}
	else
	{
		//将offset结果纳入array;
		int nNum = rslArray.length();  
		if(nNum == 0)
		{
			addLayer(_T("problem"));
			pPlineOrg->upgradeOpen();
			pPlineOrg->setLayer(_T("problem"));  
			pPlineOrg->downgradeOpen();  
			return;
		}

		//判断offset变大变小是否正确;
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

			//删除多义线;
			pPlineOrg->upgradeOpen();  
			pPlineOrg->erase();
			pPlineOrg->downgradeOpen();  
		}
		else //不正常的多义线及结果;
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

	//判断圆的半径是否适合做offset
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
		//pCircle->close();  //here，don't close it;   
		return ;
	}
}


//检查是否为多义线,如果是多义线是否闭合,零宽度?
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

	//is zero width? 需要检查这个吗?
	return true;  
}


//检查offset方向是否正确；通过检测offset后的图形最外框大小，及原图形最外框大小，对比两者；
//true 代表 正确ok;
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

	//判断是否同时为正，或同时为负; 
	double xDelt = newExt.maxPoint().x - orgExt.maxPoint().x;  
	if((xDelt > 0 && dblOfstSize > 0) ||(xDelt < 0 && dblOfstSize <0))
	{
		return true;
	}
	else  //其他情况一律判错;  
	{
		return false;
	}
}


//对选择集中的多义线执行offset;
void
CPlineOffset::ofst(IN ads_name ss,OUT AcDbVoidPtrArray& m_plineArr)
{
	if(m_offsetDist == 0)  //偏移值等于0，没有意义;
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


//闭合多义线偏移命令对应的函数
void
ofstPline()
{
	ads_name ss;
	select(ss);

	ads_real dblOfstDist = 0;  //offset偏移值;
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

	//多义线加入数据库;
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
//多义线指针为空，则返回false;
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
//如果多义线不闭合，检查其起点和终点是否重合.
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
	objTol.setEqualPoint(0.000001); //误差值设为6位小数;
	pline->getStartPoint(ptStart);
	pline->getEndPoint(ptEnd);
	if(ptStart.isEqualTo(ptEnd,objTol))
	{
		return ASSUME_CLOSED;
	}
	return NOT_CLOSED;
}



//求多义线某个顶点处的vector3d
//对每个边求得2个vector，一个起点处，一个终点处;
//最后只需要两两vector求夹角，就可以求得每个顶点处的夹角;
//此为求多义线所有夹角的算法;
void 
CIsPlineCCW::calVectorOfVertex(IN const int iIndex)
{
	AcGeVector3d kxVec(1,0,0);  //定义x轴

	AcGeVector3d vect3d;
	AcGePoint3d ptStart;
	AcGePoint3d ptEnd;
	AcGePoint3d ptCenter;
	
	int iIndex1 = iIndex; //第一个顶点;
	int iIndex2 = (iIndex1 + 1) % m_nNumVerts; //第二个顶点;

	//最后一个顶点,退出;
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
		m_allVec3d.push_back(vect3d);  //得到线段的向量;
		m_allVec3d.push_back(vect3d);  //至线段也要加2次
	}
	else if(m_pline->segType(iIndex1) == AcDbPolyline::kArc)
	{
		//求弧段在起点处的vector;
		double dblBugle = 0;
		m_pline->getBulgeAt(iIndex1,dblBugle);
		AcGeCircArc3d segArc3d;
		m_pline->getArcSegAt(iIndex1,segArc3d);
		ptCenter =  segArc3d.center();
		AcGeVector3d  vec1;
		AcGeVector3d vec2;
		
		//弧段中心到起始端点的向量;
		double x = ptStart.x - ptCenter.x;
		double y = ptStart.y - ptCenter.y;
		double z = ptStart.z - ptCenter.z;
		vec1.set(x,y,z);

		//求弧段在终点处的vector
		x = ptEnd.x - ptCenter.x;
		y = ptEnd.y - ptCenter.y;
		z = ptEnd.z - ptCenter.z;
		vec2.set(x,y,z);
		//对向量进行旋转，获得端点的向量;
		//根据bugle正负判断弧段是逆时针还是顺时针，从而对向量进行对应旋转;
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


//获取所有顶点的vector;
//最后一个顶点不计算，因为它的前一个顶点已经计算过了;
void
CIsPlineCCW::getAllVecOfVtx()
{
	for(int i = 0; i < m_nNumVerts; i++) //最后一个顶点可以计算，即使它是无效顶点;
	{
		calVectorOfVertex(i);
	}

	//求得所有vector后，将最后一个vector移动到最前面;以便后面两两取得vector计算夹角;
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


//根据vector3d集合，获取每个顶点处夹角;
//夹角以第一条边开始，第二条边结束；边的顺序按其在多义线中的顺序为准;
//起点和终点要特别处理;
void
CIsPlineCCW::getAllAngles()
{
	getFirstAng(); //第一个顶点特殊处理;
	
	//处理其他顶点的角度;
	double dblCrossProductNum = 0;
	int iIndexStart;
	int iIndexEnd;
	AcGeVector3d vec1;
	AcGeVector3d vec2;
	AcGeVector3d vec3;
	int nNum = m_allVec3d.size();
	for(int i = 2; i+1 < nNum; i+=2 ) //0和1是第一个顶点；从2开始;
	{
		iIndexStart = i;
		iIndexEnd = i+1;
		vec1 = m_allVec3d[iIndexStart];
		vec2 = m_allVec3d[iIndexEnd];
		vec3 = vec1.crossProduct(vec2);
		 dblCrossProductNum = vec3.z;  //z坐标的值代表面积，符号代表vec1到vec2是转交逆时针还是顺时针;
		if(abs(dblCrossProductNum) > m_tol)  //等于0的角度不收集;
		{
			m_crossProductNum.push_back(dblCrossProductNum);
		}
	}
}


//计算第一个顶点的夹角,第一个顶点的夹角要特殊处理;
//向量系列已经整理过了,直接两两取向量计算夹角即可;
void
CIsPlineCCW::getFirstAng()
{
	const int nNumOfVec = m_allVec3d.size();  //

	double dblCrossProductNum = 0;
	int iIndexFirst = 0;
	int iIndexSecond = 1;
	
	AcGeVector3d vec1 = m_allVec3d[iIndexFirst];
	AcGeVector3d vec2 = m_allVec3d[iIndexSecond];	
	
	if(m_isClosed != NOT_CLOSED)  //多义线不闭合;
	{
		dblCrossProductNum = 0;
		return;
		//m_allAngels.push_back(dblAng);
	}
	else  //闭合有两种情况，一样处理;
	{
		AcGeVector3d vec3 = vec1.crossProduct(vec2);
		dblCrossProductNum = vec3.z;
		//m_allAngels.push_back(dblAng);s
	}

	//整理dblAng;  角度为0则不收集.
	if(abs(dblCrossProductNum) <= m_tol)
	{
		dblCrossProductNum = 0;
		return;
	}
	m_crossProductNum.push_back(dblCrossProductNum);
}


//多义线逆时针判定
//通过多义线顶点处相邻两条边（向量）都叉积的z坐标值的正负和数量来判定是否逆时针;
//正夹角的个数多余负夹角的个数，则为逆时针;
//规则：去掉连续重复的正值，或者负值,只保留一个;
//集合最后一个数的正负要同第一个数的正负关联考虑，如果相同，也要减少相应计数;
EIsPlineCCW
CIsPlineCCW::isCCW()
{
	int nNum = m_crossProductNum.size();
	if(nNum <= 0)
	{
		return ERROR_PLINE;
	}

	int nNumPos = 0; //正角度的个数;
	int nNumNeg = 0; //负角度的个数;
	bool bPre = true; //前一个数的状态:正负;
	bool bNow = true;
	double dblAng = 0;
	//第一个数的状态;
	dblAng = m_crossProductNum.front();
	bPre = dblAng > 0? true:false;
	bPre == true? nNumPos++:nNumNeg++;
	//其他数的状态及统计;
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

	//判断
	return (nNumPos > nNumNeg? UNCLOCKWISE_PLINE:CLOCKWISE_PLINE);

	//经过分析，以下作废，不需要；
	//第一个数和最后一个数要比较状体，如果相同个，则相应计数要减去一;
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

	//if(nNumNeg > nNumPos) //会相等吗？
	//{
	//	return false;
	//}
	//else
	//{
	//	return true;  //逆时针;
	//}
}


//判断多义线是否逆时针;
EIsPlineCCW
CIsPlineCCW::isPlineCCW(IN AcDbPolyline* pPline)
{	
	if(!inputPolyline(pPline))
	{
		return ERROR_PLINE; //多义线有问题;
	}

	//取vector集合
	getAllVecOfVtx();

	//取夹角集合;
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


//取某条边的最高点状态;
//bool
//CJudgePlineCCW::prsTopY(IN const int iIndexVtx,OUT struct STopY& sta)
//{
//	STopY st;
//	return st;
//}

//判断多义线是否逆时针;
//调用其他函数;
EIsPlineCCW 
CJudgePlineCCW::isPlineCCW()
{
	//判断多义线是否存在
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


//当边是line时,取最高点状态;
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


////当边是arc时,取最高点状态;    //old version,give up;
//bool
//CJudgePlineCCW::prsTopYArcSeg(IN const int iIndexVtx,OUT struct STopY& sta)
//{
//	if(m_pline->segType(iIndexVtx) != AcDbPolyline::kArc)
//	{
//		return false;
//	}
//
//	//求bugle,用来判断弧段是逆时针还是顺时针;
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
//	//判断最高点是不是在弧中间;
//	double angleStart = vec1.angle();    
//	double angleEnd = vec2.angle();   
//	double angleMid = Pi / 2.0;  
//	bool isMidHigher = false;
//	if((angleEnd - angleStart) > TOL_CCW)  //大于
//	{
//		if((angleMid - angleStart) > TOL_CCW && (angleEnd - angleMid) < TOL_CCW) 
//		{
//			isMidHigher = true;
//		}
//	}
//	else if((angleEnd - angleStart) < TOL_CCW)  //起始角度大于终点角度;说明终点角度大于360度;
//	{
//		if((angleMid - angleStart) > TOL_CCW || (angleEnd - angleMid) < TOL_CCW) 
//		{
//			isMidHigher = true;  
//		}
//	}
//	else  //不会发生;
//	{
//		acutPrintf(_T("\nThis arc is zero degree! error arc..."));  
//		isMidHigher = true;
//	}
//
//	//判断最高点;
//	if(isMidHigher == true)
//	{
//		sta.m_iIndexVtx = iIndexVtx;
//		sta.m_pt2d.set(ptCenter.x,ptCenter.y+dblRadiu); 
//		sta.m_topYLocation = TOPY_IN_MIDDLE;
//		sta.m_y = ptCenter.y + dblRadiu;
//		return true;
//	}
//	else  //判断弧的端点;  
//	{
//		//求起点坐标
//		vec1.set(dblRadiu,0); 
//		vec1.rotateBy(angleStart);
//		ptStart.set(ptCenter.x + vec1.x, ptCenter.y+ vec1.y);
//
//		vec1.set(dblRadiu,0);
//		vec1.rotateBy(angleEnd);
//		ptEnd.set(ptCenter.x + vec1.x, ptCenter.y+vec1.y);
//
//		if((ptStart.y - ptEnd.y) >= TOL_CCW) //起点高或者等高;
//		{
//			sta.m_iIndexVtx = iIndexVtx;
//			sta.m_pt2d = ptStart;
//			sta.m_topYLocation = (dblBugle > 0)? TOPY_ON_START:TOPY_ON_END; //AcGeCircArc2d本身走向和"多义线弧段"不一定相同;
//			sta.m_y = ptStart.y;
//			return true;
//		}
//		else //if((ptEnd.y - ptStart.y ) > TOL_CCW)
//		{
//			sta.m_iIndexVtx = iIndexVtx;
//			sta.m_pt2d = ptEnd;
//			sta.m_topYLocation = (dblBugle > 0)?TOPY_ON_END:TOPY_ON_START; //AcGeCircArc2d本身走向和"多义线弧段"不一定相同;
//			sta.m_y = ptEnd.y;
//			return true;
//		}		
//	}
//	return true;
//};


//当边是arc时,取最高点状态;
//先判断最高点是否和端点重合； 再判断最高点是否在弧段中间；
//然后判断端点是否最高点;
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
	AcGePoint2d ptHighst(ptCenter.x,ptCenter.y + dblRadiu); //最高点坐标;
	
	AcGeTol objTol;
	objTol.setEqualPoint(TOL_CCW);
	//判断最高点是否和端点重合;
	if(ptHighst.isEqualTo(ptStart,objTol))
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptStart;
		sta.m_topYLocation = TOPY_ON_START; //AcGeCircArc2d本身走向和"多义线弧段"不一定相同;
		sta.m_y = ptStart.y;
		return true;
	}
	else if(ptHighst.isEqualTo(ptEnd,objTol))
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptEnd;
		sta.m_topYLocation = TOPY_ON_END; //AcGeCircArc2d本身走向和"多义线弧段"不一定相同;
		sta.m_y = ptEnd.y;
		return true;
	}

	//判断最高点是否在弧段上; 且不与端点重合; 如果重合，则上面已经处理了;
	Adesk::Boolean isHigstPtOnArc = false;	
	isHigstPtOnArc = carc2d.isOn(ptHighst,objTol);
	if(isHigstPtOnArc == kTrue)
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptHighst;
		sta.m_topYLocation = TOPY_IN_MIDDLE; //AcGeCircArc2d本身走向和"多义线弧段"不一定相同;
		sta.m_y = ptHighst.y;
		return true;
	}	

	//最高点不在弧段上，则端点中的一个为最高点;
	if((ptStart.y - ptEnd.y) >= TOL_CCW) //起点高或者等高;
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptStart;
		sta.m_topYLocation = TOPY_ON_START; //AcGeCircArc2d本身走向和"多义线弧段"不一定相同;
		sta.m_y = ptStart.y;
		return true;
	}
	else //if((ptEnd.y - ptStart.y ) > TOL_CCW)
	{
		sta.m_iIndexVtx = iIndexVtx;
		sta.m_pt2d = ptEnd;
		sta.m_topYLocation = TOPY_ON_END; //AcGeCircArc2d本身走向和"多义线弧段"不一定相同;
		sta.m_y = ptEnd.y;
		return true;
	}	
}


//判断弧段最高点是不是在弧段中间，而非端点;
//bool
//CJudgePlineCCW::isHigherPtOn(IN const double startAng,IN const double endAng,IN const double dblBug)
//{
//	;
//}


//求直线段的最高点
//返回值：true 有效； false：无效;
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
	//判断y值;
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


//获取最高点
bool
CJudgePlineCCW::prsTopY(OUT struct STopY& sta)
{
	AcGeLineSeg2d geLine2d;
	AcGeCircArc2d geArc2d;

	int nNumVerts = m_pline->numVerts();  

	//获取第一个sta;
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

	//有了第一个sta后，可以进行后续比较了;
	struct STopY staNow;
	for(int i = j; i < nNumVerts; i++)
	{
		if(m_pline->segType(i) == AcDbPolyline::kLine)
		{
			prsTopYLineSeg(i,staNow);
			if((staTemp.m_y - staNow.m_y) < TOL_CCW) //比较y坐标;  
			{
				staTemp = staNow;
			}
		}
		else if(m_pline->segType(i) == AcDbPolyline::kArc)
		{
			prsTopYArcSeg(i,staNow);
			if((staTemp.m_y - staNow.m_y) < TOL_CCW) //比较y坐标
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


//当最高线段是一条水平线的时候；
//根据该水平线判断顺逆时针;
//角度是0，表示顺时针；角度是pi，表示逆时针;  
EIsPlineCCW
CJudgePlineCCW::whenAllLine(const STopY& sta)
{
	if(sta.m_topYLocation != TOPY_ALL_LINE)
	{
		return ERROR_PLINE;
	}

	//此线段为水平线;
	AcGeLineSeg2d lnseg2da;
	AcGePoint2d pt1;
	AcGePoint2d pt2;
	AcGeLineSeg2d lnseg2d;
	m_pline->getLineSegAt(sta.m_iIndexVtx,lnseg2d);
	pt1 = lnseg2d.startPoint();
	pt2 = lnseg2d.endPoint();
	if(pt1.x > pt2.x)  //180度
	{
		return UNCLOCKWISE_PLINE;  //顺时针;
	}
	else if(pt1.x < pt2.x)
	{
		return CLOCKWISE_PLINE; //逆时针; 
	}
	else   //若pt1.x == pt2.x，说明多义线有问题(没有整理好);
	{ 
		return ERROR_PLINE;
	}	
}


//最高点在弧段上，非端点;
//判断弧段顺逆时针即可; 其代表整条多义线的走向;
EIsPlineCCW
CJudgePlineCCW::whenPtOnMiddle(const STopY& sta)
{
	//判断是否
	if(sta.m_topYLocation != TOPY_IN_MIDDLE)
	{
		return ERROR_PLINE;
	}

	int iIndex = sta.m_iIndexVtx;
	//判断此弧段走向;
	AcGeCircArc2d arc2da;
	double dblBugle = 0;
	m_pline->getBulgeAt(iIndex,dblBugle);
	return (dblBugle >0)? UNCLOCKWISE_PLINE:CLOCKWISE_PLINE;
}


//最高点在line的起点;
//先求取上一条弧段，再计算两个弧段的角度，比较两个角度，得出多义线走向;
//返回：是否逆时针;
//bCheckType：检查sta是否TOPY_ON_START
EIsPlineCCW
CJudgePlineCCW::whenPtOnStart(const STopY& sta)  
{
	//check
	if(sta.m_topYLocation != TOPY_ON_START)  
	{
		return ERROR_PLINE; 
	}

	//求得上一条线段信息; 
	//并求得其角度; 
	int numTotalVtxs = 0;  
	numTotalVtxs =  m_pline->numVerts();  
	STopY stb; 
	stb.m_iIndexVtx = (sta.m_iIndexVtx - 1 + numTotalVtxs) % numTotalVtxs;  //防止得到负数;   
	stb.m_pt2d = sta.m_pt2d;
	stb.m_topYLocation = TOPY_ON_END;  
	stb.m_y = sta.m_y;

	double anglea = -1;
	double angleb  = -1;
	anglea = calRadian(stb);  
	angleb = calRadian(sta);  
	//角度值进一步整理：不能等于0(修改为2Pi);  
	anglea = anglea < TOL_CCW ? 2*Pi:anglea; 
	angleb = angleb < TOL_CCW ? 2*Pi:angleb; 
	if(angleb > anglea)
	{
		return CLOCKWISE_PLINE; //顺时针   
	}
	else if(angleb < anglea)   
	{
		return UNCLOCKWISE_PLINE;   
	}

	return ERROR_PLINE;  //
}



//最高点在line的起点;
//先求取上一条弧段，再计算两个弧段的角度，比较两个角度，得出多义线走向;
//返回：是否逆时针;
//bCheckType：检查sta是否TOPY_ON_START
EIsPlineCCW
CJudgePlineCCW::whenPtOnEnd(const STopY& sta)
{
	//check
	if(sta.m_topYLocation != TOPY_ON_END)
	{
		return ERROR_PLINE; 
	}	

	//求得上一条线段信息; 
	//并求得其角度; 
	int numTotalVtxs = 0;
	numTotalVtxs = m_pline->numVerts();
	STopY stb; 
	stb.m_iIndexVtx = (sta.m_iIndexVtx + 1) % numTotalVtxs;  //防止超出范围;
	stb.m_pt2d = sta.m_pt2d; 
	stb.m_topYLocation = TOPY_ON_START;  
	stb.m_y = sta.m_y;

	double anglea = -1;
	double angleb  = -1;
	anglea = calRadian(sta);
	angleb = calRadian(stb);
	//角度值进一步整理：不能等于0(修改为2Pi);  
	anglea = anglea < TOL_CCW ? 2*Pi:anglea; 
	angleb = angleb < TOL_CCW ? 2*Pi:angleb; 
	if(angleb - anglea > TOL_CCW)  //大于;
	{
		return CLOCKWISE_PLINE; //顺时针
	}
	else if(angleb - anglea < (0 - TOL_CCW))
	{
		return UNCLOCKWISE_PLINE;
	}

	return ERROR_PLINE;  //
}


//根据最高点情况，求该弧段角度;
//不针对TOPY_IN_MIDDLE情况;其他情况可以运算;
//注意区分弧和线;
//返回角度值； 返回-1表示失败;
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
		m_pline->getBulgeAt(sta.m_iIndexVtx,dblBug);  //判断弧段是逆时针还是顺时针.
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

	return -1;  //角度是没有负数的，返回-1表示失败;
}


//=============================
//class CIsPlineCCW test function
//=============================
//调用类CIsPlineCCW，测试多义线是逆时针否。
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
//调用类CJudgePlineCCW，测试多义线是逆时针否。
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