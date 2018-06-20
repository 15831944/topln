/************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
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
#include "..\StdAfx.h"
#include "TCHAR.h"

#include "pointMap.h"


//
SXData::SXData()
{
}

SXData::~SXData()
{
}


//copy constructor
SXData::SXData(IN const SXData& src)
{
	m_x = src.m_x;
	m_pPointMap = src.m_pPointMap;
	m_nDotNum = 5;
}


//insert
map<double,SYData,dblcmp>::iterator
SXData::insert(IN const double yVal,IN const int ptInex)
{
	pair<map<double,SYData,dblcmp>::iterator,bool> pairRtn;
	map<double,SYData,dblcmp>::iterator itrY;
	bool bflag = false;
	SYData stY;
	stY.m_y = yVal;
	stY.pt.set(m_x,yVal,0);
	stY.m_PointIndex = ptInex;
	pairRtn = m_pPointMap.insert(pair<double,SYData>(yVal,stY));
	itrY = pairRtn.first;
	return itrY;
}


//find
//不能直接find，必须用upper_bound和lower_bound;
bool
SXData::find(IN const double yVal,OUT int& ptIndex)
{
	
	map<double,SYData,dblcmp>::iterator itr = m_pPointMap.find(yVal);
	
	if(itr != m_pPointMap.end())  //
	{			
		ptIndex = itr->second.m_PointIndex;
		return true;		
	}

	ptIndex = -1;
	return false;
}


//erase
unsigned int
SXData::erase(IN const double yVal)
{
	unsigned int uNum;
	uNum = m_pPointMap.erase(yVal);
	return uNum;
}


//测试，打印信息;
void
SXData::print()
{
	#ifdef DEBUG_TO_PL_PRINT

	map<double,SYData,dblcmp>::iterator itr = m_pPointMap.begin();
	AcGePoint3d pt;
	SYData stY;
	for(; itr != m_pPointMap.end(); itr++)
	{
		acutPrintf(_T("\n(%15f,%15f)%d"),itr->second.pt.x,itr->second.pt.y,itr->second.m_PointIndex);
	}

	#endif  DEBUG_TO_PL_PRINT
}


//在SDataX中查找和输入坐标距离小于dist的点；做成点对； 
//返回值：如果x1和x2距离超过dist，返回false；其他返回true;  
//注意：如果x1 == x2 则只需要向下查找y； 否则，应该双向查找y；  

bool
SXData::chkLessDistPoints(IN const double dist,IN const double xcoord,IN const SYData syData,OUT vector<pair<void*,void*>>& vPointPairs)
{
	double x1 = 0;   //本地坐标x
	double x2 = 0;   //输入坐标x;
	double y1 = syData.m_y; //第一个点坐标y;   
	double y2 = 0; //第二点坐标y;	
	x1 = m_x;
	x2 = xcoord;
	if(abs(x1-x2) > dist) 
	{
		return false;   
	}

	//x1 == x2  ;只向下寻找更大的y值;    
	if(isEqual(x1,x2))    
	{
		map<double,syData,dblcmp>::iterator itrYc = m_pPointMap.begin();       
		while(itrYc != m_pPointMap.end())  
		{
			itrYc = m_pPointMap.lower_bound(y1);      
			y2 = itrYc->second.m_y;
			if(isDistGreater(x1,y1,x2,y2,dist))
			{
				break;
			}
			else
			{
				pair<void*,void*> pairData(syData.m_dataVoidPtr,itrYc->second.m_dataVoidPtr);
				vPointPairs.push_back(pairData);
			}
		} 
	}
	else  //x1 != x2,向上及向下寻找y值;    
	{
		map<double,syData,dblcmp>::iterator itrYc = m_pPointMap.begin();       
		while(itrYc != m_pPointMap.end())  
		{
			itrYc = m_pPointMap.lower_bound(y1);      
			y2 = itrYc->second.m_y;
			if(isDistGreater(x1,y1,x2,y2,dist))
			{
				break;
			}
			else
			{
				pair<void*,void*> pairData(syData.m_dataVoidPtr,itrYc->second.m_dataVoidPtr);
				vPointPairs.push_back(pairData);
			}
		} 
	}
	
}


//=============================
//SXData struct
//=============================
//constructor
SYData::SYData()
{
}


//destructor
SYData::~SYData()
{
}


//copy constructor
SYData::SYData(IN const SYData& src)
{
	m_y = src.m_y;
	pt = src.pt;
	m_PointIndex = src.m_PointIndex;
}


//constructor
CPointMap::CPointMap()
{
	m_nDotNum = 5;  //保留5位小数;
}


//destructor
CPointMap::~CPointMap()
{
}


//弃用.
//对double变量取指定小数位;
//返回：改造后的double变量;
//这种改造是不精准的，并不能完全消除两个“相等”的double变量误差；
//比如7.0和6.99999，在这里没法改造成相等;
//所以这个函数要弃用！没卵用.
double
CPointMap::transByDotNum(IN const double xyVal,IN const int nDotNum)
{
	double baseNum = pow(10.0,nDotNum);
	long lgTotal = floor(xyVal * baseNum);
	double val = lgTotal / baseNum;  //修剪成为指定位数的小数;
	//double val = (double)lgTotal;
	return val;
}


//在误差范围内，判断俩数值是否相等
bool
CPointMap::isEqual(IN const double d1,IN const double d2,IN const int m_nDotNum)
{
	double dTol = 1.0 / (pow(10.0,m_nDotNum));  //	误差值;
	double dRes = d1 - d2;
	if(abs(dRes) <= dTol)
	{
		return true;
	}
	return false;
}


//插入：先插入x-map，再插入y-map; 
//插入之前不需要先查找是否存在; 
//*但是，因为存储的是double类型key，所以要预先查找是否存在然后插入;
//ptIndex: 顶点序号;
//返回：void;
void
CPointMap::insert(IN const double x,IN const double y,IN const int ptIndex)
{
	//double xf = transByDotNum(x,m_nDotNum);
	//double yf = transByDotNum(y,m_nDotNum);
	double xf = x;
	double yf = y;

	//插入x值;
	map<double,SXData,dblcmp>::iterator itrRtnX;
	bool bFlag = false;
	pair<map<double,SXData,dblcmp>::iterator,bool> pairRtnX; 
	SXData sx;
	sx.m_x = xf;
	pairRtnX = m_mapXcoord.insert(pair<double,SXData>(xf,sx));  
	itrRtnX = pairRtnX.first;

	//插入y;
	map<double,SYData,dblcmp>::iterator itrY;
	itrY = itrRtnX->second.insert(yf,ptIndex);  //这里insert是SXData的成员函数;
}


//insert
void
CPointMap::insert(IN const AcGePoint3d pt,IN const int ptIndex)
{
	insert(pt.x,pt.y,ptIndex);
}


//查找：查找2次，第一次查x，第二次查y;
//因为key是double类型，所以不能依靠等于运算来查找，应该依靠upper_bound和lower_bound;
//返回：1.是否查找到了;
bool
CPointMap::find(IN const double x,IN const double y,OUT int& ptIndex)
{
	//double xc = transByDotNum(x,m_nDotNum);  //修剪成为指定位数的小数;
	//double yc = transByDotNum(y,m_nDotNum);
	double xc = x ;
	double yc = y ;	

	//acutPrintf(_T("\n(%15f,%15f)"),xc,yc);

	map<double,SXData,dblcmp>::iterator itr =  m_mapXcoord.find(xc); //
	if(itr != m_mapXcoord.end())  //对x进行查找.  
	{		
		if(itr->second.find(yc,ptIndex))
		{
			return true;  //如果找到就返回; 
		}
	}
	return false;  //运行到这里说明没找到.
}


//find
bool
CPointMap::find(IN const AcGePoint3d pt,OUT int& ptIndex)
{
	return find(pt.x,pt.y,ptIndex);
}


//set dot number
//返回：true说明设置成功； false说明设置失败; 失败原因：输入不正确或已经设置过了;
bool
CPointMap::setDotNum(IN const int nDotNum)
{
	if(nDotNum <= 0 || isDotNumSet == true)
	{
		return false;
	}
	else
	{
		m_nDotNum = nDotNum;
		return true;
	}
}


//test print
void
CPointMap::print()
{
#ifdef DEBUG_TO_PL_PRINT	 
	map<double,SXData,dblcmp>::iterator itr = m_mapXcoord.begin();
	for(; itr != m_mapXcoord.end(); itr++)
	{
		itr->second.print();
	}
#endif
}


//寻找距离小于dist的点对
void
CPointMap::findPointPairs(IN const double dist,OUT vector<pair<void*,void*>> pointPairs)
{
	map<double,SXData,dblcmp>::iterator itrxFirst = m_mapXcoord.begin(); //取出的x列，用来对比；
	map<double,SXData,dblcmp>::iterator itrxNext;    //取出的x+n列，用来被对比; x + n > x;
	map<double,SYData,dblcmp>::iterator itry;  //取得y坐标;
	
	//three level loops;
	//取得x坐标;
	double mx = 0;
	double my = 0;
	SYData syData;
	void* voidDataPtr = NULL;
	bool flag = false;  //判断是否不用继续查找下一个sxdata; 
	for(; itrxFirst != m_mapXcoord.end(); itrxFirst++) 
	{
		mx = itrxFirst->second->m_x;
		//继续取得y坐标及挂载数据;
		itry = itrxFirst->second()->syDataBegin();		
		syData = (SYData)(*itry);
		for(; itry != itrxFirst->second->syDataEnd(); itry++)
		{
			my = itry->second->m_y;
			voidDataPtr = itry->second->m_dataVoidPtr;
			//third loop
			for(itrxNext = itrxFirst; itrxNext != m_mapXcoord.end();itrxNext++)
			{
				flag = itrxNext->second->chkLessDistPoints(dist,mx,syData,pointPairs); 
				if(!flag)  //返回false，说明x1和x2距离太远了，可以退出本层循环了;
				{
					break;  
				}
			}
		}
	}
}


//test point map class
void
testPointMapClass()
{
	ads_name ss;
	ads_name ssUnit;
	if(!select(ss))
	{
		return;
	}
	CTimeElapse objTimesElpased;
	
	long nNumSS = 0;
	acedSSLength(ss,&nNumSS);  

	AcDbObjectId id;
	AcDbEntity* pEnt;

	CPointMap objPtMap;
	objPtMap.setDotNum(6);
	AcGePoint3d pt;
	for(long i = 0; i < nNumSS; i++)
	{
		acedSSName(ss,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);
		if(pEnt->isA() == AcDbLine::desc())
		{
			AcDbLine* pLine = (AcDbLine*)pEnt;
			objPtMap.insert(pLine->startPoint(),i);
			objPtMap.insert(pLine->endPoint(),i);
			pEnt->close();
		}
		else if(pEnt->isA() == AcDbPolyline::desc())
		{
			AcDbPolyline* pPline = (AcDbPolyline*)pEnt;
			int nNum = 0;
			nNum =  pPline->numVerts();
			pPline->getPointAt(0,pt);
			objPtMap.insert(pt,i);
			pPline->getPointAt(nNum-1,pt);
			objPtMap.insert(pt,i);
			pEnt->close();
		}
		else if(pEnt->isA() == AcDbArc::desc())
		{
			/*AcDbArc* pArc = (AcDbArc*)pEnt;
			objPtMap.insert(pArc->start());
			objPtMap.insert(pArc->endPoint());*/
			pEnt->close();
			continue;
		}
	}
	objPtMap.print();  //打印，测试;

	//测试寻找一个点;
	//objTimesElpased.~CTimeElapse();
	ads_point adsPoint;
	
	acedGetPoint(NULL,_T("\nPlease select a point:"),adsPoint);
	int ptIndex = -1;
	CTimeElapse objTm002;
	AcGePoint3d testAcPoint;
	testAcPoint.set(adsPoint[0],adsPoint[1],adsPoint[2]);
	if(objPtMap.find(testAcPoint,ptIndex))
	{
		#ifdef DEBUG_TO_PL_PRINT
		acutPrintf(_T("\nfind it(%f,%f,%f)%d!"),adsPoint[0],adsPoint[1],adsPoint[2],ptIndex);
		#endif
	}
	else
	{
		#ifdef DEBUG_TO_PL_PRINT
		acutPrintf(_T("\n not find it(%f,%f,%f)!"),adsPoint[0],adsPoint[1],adsPoint[2]);
		#endif
	}
	acedSSFree(ss);
}