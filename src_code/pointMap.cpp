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
SXData::insert(IN const double yVal,IN const int ptInex,OUT void* voidPtr)   
{
	pair<map<double,SYData,dblcmp>::iterator,bool> pairRtn;
	map<double,SYData,dblcmp>::iterator itrY;
	bool bflag = false;
	SYData stY;
	stY.m_y = yVal;    
	stY.pt.set(m_x,yVal,0);
	stY.m_PointIndex = ptInex;
	stY.m_dataAttach = voidPtr;  //附加数据;
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


//测试，打印map的所有点的信息;
void
SXData::print()
{
	#ifdef DEBUG_TO_PL_PRINT_POINTMAP

	acutPrintf(_T("\nSXData..print()-----"));
	map<double,SYData,dblcmp>::iterator itr = m_pPointMap.begin();
	AcGePoint3d pt;
	SYData stY;
	for(; itr != m_pPointMap.end(); itr++)
	{
		acutPrintf(_T("\n(%15f,%15f)%d"),itr->second.pt.x,itr->second.pt.y,itr->second.m_PointIndex);
	}
	acutPrintf(_T("\n                -----nSXData..print()"));

	#endif  DEBUG_TO_PL_PRINT_POINTMAP
}


//计算两点距离，并与输入距离相比较
//如果不大于，则要排除相等的情况;
bool
SXData::isDistGreater(IN const double x1,IN const double y1,IN const double x2,IN const double y2,IN const double dist)
{
	 double distWork = 0;
	 distWork = sqrt((x1-x2)*(x1-x2) +(y1-y2)*(y1-y2));   
	 AcGeTol objTol;
	 if(distWork > dist && abs(distWork - dist) > objTol.equalPoint())
	 {
		 return true;   
	 }
	 else
	 {
		 return false;
	 }
}


//比较俩double大小;  
//d1大于d2，返回true;
//误差值设置：
bool
SXData::isDigitGreater(IN const double d1,IN const double d2)
{
	AcGeTol objTol;
	if(d1 > d2 && abs(d1 - d2) > objTol.equalPoint())
	{
		return true;
	}
	else
	{
		return false;   
	}
}


//比较俩double大小;
//d1大于d2，返回true; 
//误差值设置：
bool
SXData::isDistZero(IN const double x1,IN const double y1,IN const double x2,IN const double y2)
{
	double distWork = 0;
	distWork = sqrt((x1-x2)*(x1-x2) +(y1-y2)*(y1-y2)); 
	AcGeTol objTol;
	if(distWork <= objTol.equalPoint())
	{
		return true;
	}
	else
	{
		return false;  
	}
}



//返回m_pPointMap遍历器起始指针;
map<double,SYData,dblcmp>::iterator 
SXData::syDataBegin()
{
	return m_pPointMap.begin();   
}


 //返回m_pPointMap遍历器结束指针;
map<double,SYData,dblcmp>::iterator 
SXData::syDataEnd()
{
	return m_pPointMap.end();
}


//判断俩实数是否相等
bool
SXData::isEqual(IN const double d1, IN const double d2)
{
	if(isDigitGreater(d1,d2) || isDigitGreater(d2,d1))
	{
		return false;
	}
	else
	{
		return true;
	}
}


//在SDataX中查找和输入坐标距离小于dist的点；做成点对； 
//返回值：如果x1和x2距离超过dist，返回false；如果两个点距离等于0，返回false； 其他返回true;  
//注意：如果x1 == x2 则只需要向下查找y； 否则，应该双向查找y；  
bool
SXData::chkLessDistPoints(IN const double dist,IN const double xcoord,IN const SYData syData,OUT vector<pair<void*,void*>>& vPointPairs)
{
	double x1 = 0;   //（输入）源坐标点x； 
	double x2 = 0;   //目标坐标点x;  
	double y1 = syData.m_y; //（输入）源坐标点y;       
	double y2 = 0; //目标坐标点y;	
	double ytemp = y1; //temp y;     
	x1 = xcoord;   
	x2 = m_x;  
	if(abs(x1-x2) > dist)     //x方向距离已经大于dist; 
	{
		return false;     
	}

	//x1 == x2  ;只向下寻找更大的y值;    
	map<double,SYData,dblcmp>::iterator itrYc; // = m_pPointMap.begin();	 
	if(isEqual(x1,x2))    
	{
		itrYc = m_pPointMap.lower_bound(ytemp);     
		while(itrYc != m_pPointMap.end()) 
		{			      
			y2 = itrYc->second.m_y;  		
#ifdef DEBUG_TO_PL_PRINT_POINTMAP
			acutPrintf(_T("\n 比较点对："));
			acutPrintf(_T("(%5.5f,%5.5f),  (%5.5f,%5.5f)"),x1,y1,x2,y2);  
#endif  DEBUG_TO_PL_PRINT_POINTMAP
			
			if(isDistGreater(x1,y1,x2,y2,dist))     
			{
				break;     
			}
			else if(isDistZero(x1,y1,x2,y2))     
			{
				itrYc++; 
				continue;   
			}
			else
			{
				pair<void*,void*> pairData(syData.m_dataAttach,itrYc->second.m_dataAttach);  
				vPointPairs.push_back(pairData);   
				itrYc++;   
			}   
		} 
	}
	else  //x1 != x2,向上及向下寻找y值;    
	{
		//先向上找:不能漏了begin（）这个元素;
		itrYc = m_pPointMap.lower_bound(ytemp); 
		bool isBreak = false;
		//因为现在itrYc的y值是not less than y1,所以先向上（y值更小的方向）退一步;
		//itrYc--;
		while(true)   //有可能itrYc直接到了end;
		{	
			//if(itrYc == m_pPointMap.end()) //不能让itrYc停留在end处，会崩溃;  
			//{
			//	itrYc--;
			//}
			if(itrYc == m_pPointMap.begin())
			{
				isBreak = true; //执行完此轮就退出循环;  
			}
			else  //including the end;
			{
				itrYc--;
			}

			y2 = itrYc->second.m_y;      // 调试：到这里崩溃； x1=0；y1=0.001； x2=0.001；y2应该等于0;
#ifdef DEBUG_TO_PL_PRINT_POINTMAP
			acutPrintf(_T("\n 比较点对："));
				acutPrintf(_T("(%5.5f,%5.5f),  (%5.5f,%5.5f)"),x1,y1,x2,y2);  
#endif  DEBUG_TO_PL_PRINT_POINTMAP
			if(isDistGreater(x1,y1,x2,y2,dist))   
			{
				break;  
			}  
			else if(isDistZero(x1,y1,x2,y2))   
			{
				itrYc--;  
				continue;   
			}
			else  
			{
				pair<void*,void*> pairData(syData.m_dataAttach,itrYc->second.m_dataAttach);    
				vPointPairs.push_back(pairData);   
				itrYc--;
			}

			if(isBreak)  //break;
			{
				break;
			}
		} 

		//再向下找; 
		itrYc = m_pPointMap.lower_bound(ytemp);		
		//if(itrYc != m_pPointMap.end())
		//{
		//	itrYc++;//这元素是和向上查找重叠的;所以要剔除;
		//}
		while(itrYc != m_pPointMap.end())  
		{			  
			y2 = itrYc->second.m_y;
#ifdef DEBUG_TO_PL_PRINT_POINTMAP
			acutPrintf(_T("\n 比较点对："));
			acutPrintf(_T("(%5.5f,%5.5f),  (%5.5f,%5.5f)"),x1,y1,x2,y2);
#endif  DEBUG_TO_PL_PRINT_POINTMAP
			ytemp = y2;
			if(isDistGreater(x1,y1,x2,y2,dist))  
			{
				break;  
			}
			else if(isDistZero(x1,y1,x2,y2))     
			{
				itrYc++;  
				continue; 
			}
			else  
			{
				pair<void*,void*> pairData(syData.m_dataAttach,itrYc->second.m_dataAttach);   
				vPointPairs.push_back(pairData);    
				itrYc++;    
			}
		} 
	}	

	return true;  //除了x2-x1>dist,其它情况返回true; 
}



//寻找小于dist距离的点对
//vPointPairs:装载点对数据;
bool 
SXData::findPointPairsLessDist(OUT const vector<pair<void *, void *>>* vPointPairs)
{
	vPointPairs->clear();
	bool bflag001 = false;
	bflag001 = searchUpperByYVal(vPointPairs);

	bool bflag002 = false;
	bflag002 =  searchDownByYVal(vPointPairs);

	return (bflag001 && bflag002);  
}


//根据y值向上寻找小于dist距离的点；
//返回值：true表示找到了一些符合要求的点； false表示没有找到符合要求的点;
bool
SXData::searchUpperByYVal(IN const vector<double, SYData, dblcmp>::iterator itrFistSyData)
{
	bool bResult = false;
	vector<double,SYData,dblcmp>::iterator itrNext  =  m_pPointMap.lower_bound(m_firstY);	
	for(; itrNext != m_pPointMap.end(); itrNext++)
	{
		if(((SYData)(*itrNext)).isDistSmaller((SYData)(m_FirstSyDataToComp)))
		{
			;
			bResult = true;
			continue;
		}
		else
		{
			break;  
		}
	}	
	return bResult; 
}



//=============================
//struct SAttachData
//=============================
//初始化，或者赋值；
void
SAttachData::init(const AcGePoint3d& pt,const AcDbEntity* pEnt)
{
	m_pt3d = pt;
	m_entId = pEnt->objectId();
}


//打印信息;
void
SAttachData::print()
{
#ifdef DEBUG_TO_PL_PRINT_POINTMAP
	acutPrintf(_T("\nSAttachData point-------"));
	acutPrintf(_T("\n(%15f,%15f)"),m_pt3d.x,m_pt3d.y);
	acutPrintf(_T("\n                 -------SAttachData point"));
#endif
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
	m_dataAttach = src.m_dataAttach;
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
CPointMap::insert(IN const double x,IN const double y,IN const int ptIndex,IN  void* voidPtr)
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
	itrY = itrRtnX->second.insert(yf,ptIndex,voidPtr);  //这里insert是SXData的成员函数;
}


//insert
void
CPointMap::insert(IN const AcGePoint3d pt,IN const int ptIndex,IN void* voidPtr)
{
	insert(pt.x,pt.y,ptIndex,voidPtr);   
}


//删除一个元素
//分两层删除，先找到x，然后删除y；然后判断x是否为空，空就删除x;
//返回值：true-删除成功； false-删除不成功,可能该元素不存在;
bool
CPointMap::erase(IN const double x,IN const double y)
{
	//if(m_mapXcoord.empty())
	//{		
	//	return false;  //空的;
	//}

	//find SXData
	map<double,SXData,dblcmp>::iterator itrX; // = m_mapXcoord.begin();	
	itrX = m_mapXcoord.find(x);
	if(itrX == m_mapXcoord.end())  
	{
		return false;  //没找到，不删除；返回false；	
	}

	//找y;
	SXData objXdata = (SXData)(itrX->second);    
	int iIndexPt;
	bool bFlag = false;
	bFlag = objXdata.find(y,iIndexPt);   
	if(bFlag) 
	{
		objXdata.erase(y);	
		return true;
	}
	return false;
}


//删除某个元素
bool
CPointMap::erase(IN const AcGePoint3d pt)
{
	return erase(pt.x,pt.y);
}


//ptIndex:点的序号;
//查找：查找2次，第一次查x，第二次查y;
//因为key是double类型，所以不能依靠等于运算来查找，应该依靠upper_bound和lower_bound;-内部实现;
//返回：true-找到了;false-没找到;
bool
CPointMap::find(IN const double x,IN const double y,OUT int& ptIndex,OUT void* voidPtr)
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
CPointMap::find(IN const AcGePoint3d pt,OUT int& ptIndex,OUT void* voidPtr)
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
#ifdef DEBUG_TO_PL_PRINT_POINTMAP	 
	map<double,SXData,dblcmp>::iterator itr = m_mapXcoord.begin();
	for(; itr != m_mapXcoord.end(); itr++)
	{
		itr->second.print();
	}
#endif
}


//寻找距离小于dist的点对;  
//在所有点坐标中寻找; 遍历整个坐标点集合;   
//三重循环：
void
CPointMap::findPointPairs(IN const double dist,OUT vector<pair<void*,void*>>& pointPairs)     
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
		//取得x坐标;
		mx = itrxFirst->second.m_x;   
		
		itry = itrxFirst->second.syDataBegin();
		for(; itry != itrxFirst->second.syDataEnd(); itry++)      
		{
			//继续取得y坐标及挂载数据;
			syData = (SYData)(itry->second);    
			my = itry->second.m_y;   
			voidDataPtr = itry->second.m_dataAttach;     

			//third loop：比较每一个x列;
			for(itrxNext = itrxFirst; itrxNext != m_mapXcoord.end();itrxNext++)  //
			{
				flag = itrxNext->second.chkLessDistPoints(dist,mx,syData,pointPairs);      
				if(!flag)  //返回false，说明x1和x2距离太远了，可以退出本层循环了; 
				{
					break;     
				}
			}
		}
	}
}


//打印获取的点对
void
CPointMap::printPointPairs(IN vector<pair<void*,void*>>& vPointPairs)
{
	SAttachData* pAtt = NULL;
	vector<pair<void*,void*>>::iterator itr = vPointPairs.begin();
	for(; itr != vPointPairs.end(); itr++)
	{
#ifdef DEBUG_TO_PL_PRINT_POINTMAP
		acutPrintf(_T("\n point pair -----"));   
#endif DEBUG_TO_PL_PRINT_POINTMAP
		if((void*)(itr->first) != NULL)
		{
			pAtt = ((SAttachData*)(itr->first));    
			pAtt->print();
		}
		else
		{
			acutPrintf(_T("\n CPointMap::printPointPairs: null pointer"));   
		}

		if((void*)(itr->second) != NULL)
		{
			pAtt = ((SAttachData*)(itr->second));
			pAtt->print();
		}
		else
		{
			acutPrintf(_T("\n CPointMap::printPointPairs: null pointer"));
		}
#ifdef DEBUG_TO_PL_PRINT_POINTMAP
		acutPrintf(_T("\n       -----"));   
#endif DEBUG_TO_PL_PRINT_POINTMAP
	}
}



//m_itrSXDataFirst：第一个sxData，从中取第一个点;
//m_itrSXDataNext: 第二个sxData，从中寻找和第一个点近距离的第二个点;
//m_itrSYDataFirst: 从m_itrSXDataFirst中取出的点syData;
//功能：从点集中寻找距离小于dist的点对;
bool
CPointMap::prsPointPairsLessDist(OUT vector<pair<void*,void*>>* vPtPairs)   
{
	//clean the vPtPairs
	vPtPairs->clear();

	m_itrsx = m_mapXcoord.begin();
	m_itrSXDataNext = m_itrSXDataFirst;

	m_itrSYDataFirst = m_itrSXDataFirst->second.syDataBegin();   

	for(;m_itrSXDataFirst != m_mapXcoord.end(); m_itrSXDataFirst++)   
	{
		for(; m_itrSYDataFirst != m_itrSXDataFirst->second.syDataEnd(); m_itrSYDataFirst++) 
		{
			for(;m_itrSXDataNext != m_mapXcoord.end(); m_itrSXDataNext++)   
			{
				if(m_itrSXDataNext->second.findPointPairsLessDist(dist,m_itrSYDataFirst,vPtPairs)) 
				{
					continue;
				}
				else  //m_itrSXDataNext和m_itrSXDataFirst的x距离超过dist了; 
				{
					break;
				}
			}
		}
	}
}


//=========================================
//类COptOfPointPairs;
//用来操作点对数据，比如：first，next，last;
//=========================================
//constructor
COptOfPointPairs::COptOfPointPairs()
{
	m_isInitOk = false;
	m_isItrInit = false;
}


//constructor
COptOfPointPairs::COptOfPointPairs(IN vector<pair<void*,void*>>* vPointPairs)
{
	m_ptPair = vPointPairs;
	if((m_ptPair != NULL) && m_ptPair->size() > 0)
	{
		m_isInitOk = true;
	}
	else
	{
		m_isInitOk = false;
	}
}


//destructor
COptOfPointPairs::~COptOfPointPairs()
{
}


//input the point pairs parameters
bool
COptOfPointPairs::inputPointPairs(IN vector<pair<void*,void*>>* vPointPairs)
{
	m_ptPair = vPointPairs;
	//以下为测试代码:测试m_ptPair是否有效;
	//int nlen = 0;
	//nlen = m_ptPair->size();
	//pair<void*,void*> vpairTmp;
	//vector<pair<void*,void*>>::iterator itrPair = m_ptPair->begin();  
	//acutPrintf(_T("\nOnBnClickedButtonRun--------"));
	//for(; itrPair != m_ptPair->end(); itrPair++)  
	//{
	//	vpairTmp = (pair<void*,void*>)(*itrPair); 
	//	((SAttachData*)(vpairTmp.first))->print(); 
	//	((SAttachData*)(vpairTmp.second))->print();  
	//}
	//以上为测试代码;

	if((m_ptPair != NULL) && m_ptPair->size() > 0)
	{
		m_isInitOk = true;
		return true;
	}
	else
	{
		m_isInitOk = false;
		return false;
	}	
}


//get the first point pair;
bool
COptOfPointPairs::first(OUT AcGePoint3d& pt0,OUT AcGePoint3d& pt1)
{
	double x = 0;
	double y= 0;
	double z= 0;
	if(m_ptPair->size() <= 0)   
	{
		m_isItrInit = false; 
		return false; 
	}
	else
	{
		m_itr = m_ptPair->begin();
		((SAttachData*)(m_itr->first))->print();
		((SAttachData*)(m_itr->second))->print();

		x = (((SAttachData*)(m_itr->first))->m_pt3d).x;
		y = (((SAttachData*)(m_itr->first))->m_pt3d).y;
		z = (((SAttachData*)(m_itr->first))->m_pt3d).z; 
		pt0.set(x,y,z);
			  
		x = (((SAttachData*)(m_itr->second))->m_pt3d).x;
		y = (((SAttachData*)(m_itr->second))->m_pt3d).y;
		z = (((SAttachData*)(m_itr->second))->m_pt3d).z;
		pt1.set(x,y,z);
		m_isItrInit = true;  
		return true;
	}
}


//get the next point pair;
bool
COptOfPointPairs::next(OUT AcGePoint3d& pt0,OUT AcGePoint3d& pt1)  
{
	//先判断迭代器是否初始化;
	if(m_isItrInit == true)
	{
		m_itr++;
		if(m_itr != m_ptPair->end())   
		{
			((SAttachData*)(m_itr->first))->print();
			((SAttachData*)(m_itr->second))->print();
			pt0 = ((SAttachData*)(m_itr->first))->m_pt3d;
			pt1 = ((SAttachData*)(m_itr->second))->m_pt3d;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return first(pt0,pt1);
	}	
}


//get the last point pair
bool
COptOfPointPairs::last(OUT AcGePoint3d& pt0,OUT AcGePoint3d& pt1)
{
	if(m_ptPair->empty())
	{
		return false;
	}
	else
	{
		if(m_itr != m_ptPair->end())
		{
			m_itr = m_ptPair->end();
			m_itr--;
			return true;
		}
		return false;
	}	
}




//=========================================
//类COptOfTwoPoints;
//用来操作一个点对数据,比如：放大，缩小，连接;
//=========================================
COptOfTwoPoints::COptOfTwoPoints()
{
}

//constructor
COptOfTwoPoints::COptOfTwoPoints(IN const AcGePoint3d& pt0, IN const AcGePoint3d& pt1)
{
	;
}


//
COptOfTwoPoints::~COptOfTwoPoints()
{
}


//根据两点画线;
bool
COptOfTwoPoints::drawLine()
{
	AcDbLine* objLine = new AcDbLine(m_ptStart,m_ptEnd); 
	join2database(objLine);
	return true;
}


//以两个点为范围放大视野
bool
COptOfTwoPoints::zoomThem()
{
	ads_point pt0;
	ads_point pt1;
	pt0[0] = m_ptStart.x;
	pt0[1] = m_ptStart.y;
	pt0[2] = m_ptStart.z;
	pt1[0] = m_ptEnd.x;
	pt1[1] = m_ptEnd.y;
	pt1[2] = m_ptEnd.z;
	//pt0[0] = 1;
	//pt0[1] = 1;
	//pt0[2] = 0;
	//pt1[0] = 2;
	//pt1[1] = 2;
	//pt1[2] = 2;
	acedCommand(RTSTR,_T("zoom"),RTSTR,_T("W"),RT3DPOINT,pt0,RT3DPOINT,pt1,0);  
	return true;
}



//=========================================
//执行函数，对应命令：test001;
//测试类pointMap函数;
//========================================= 
//测试COptOfTwoPoints
void testCOptOfTwoPnts::testCOptOfTwoPoints()
{
	AcGePoint3d pt1(1.0,1.0,0);
	AcGePoint3d pt2(5,5,0);
	COptOfTwoPoints objOpt(pt1,pt2);
	objOpt.drawLine();
}	



//=========================================
//执行函数，对应命令：testmap;
//也是测试类pointMap函数;
//========================================= 
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
	SAttachData* objData;  //附加数据;
	vector<SAttachData*> dataPtrVec; //收集之，用来释放动态分配的内存; 
	for(long i = 0; i < nNumSS; i++)  
	{
		acedSSName(ss,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);   
		if(pEnt->isA() == AcDbLine::desc())
		{
			objData = new SAttachData;
			dataPtrVec.push_back(objData);

			AcDbLine* pLine = (AcDbLine*)pEnt;
			objData->init(pLine->startPoint(),pEnt);
			objPtMap.insert(pLine->startPoint(),i,objData);    

			objData = new SAttachData;   
			dataPtrVec.push_back(objData);    

			objData->init(pLine->endPoint(),pEnt);
			objPtMap.insert(pLine->endPoint(),i,objData);      
			pEnt->close();   
		}
		else if(pEnt->isA() == AcDbPolyline::desc())   
		{
			objData = new SAttachData;
			dataPtrVec.push_back(objData);  

			AcDbPolyline* pPline = (AcDbPolyline*)pEnt;  
			int nNum = 0;
			nNum =  pPline->numVerts();
			pPline->getPointAt(0,pt);
			objData->init(pt,pEnt);
			objPtMap.insert(pt,i,objData);

			objData = new SAttachData;
			dataPtrVec.push_back(objData);

			pPline->getPointAt(nNum-1,pt);  
			objData->init(pt,pEnt);
			objPtMap.insert(pt,i,objData);
			pEnt->close();
		}
		else if(pEnt->isA() == AcDbArc::desc())  
		{
			AcGePoint3d pte;
			AcGePoint3d pts; 
			CSomePublicFun objFuns;
			AcDbArc* pArc = (AcDbArc*)pEnt;
			objFuns.getArcEndPoints(pArc,pts,pte);  
			
			objData = new SAttachData;
			dataPtrVec.push_back(objData);  			
			
			objData->init(pts,pEnt);
			objPtMap.insert(pts,i,objData);

			objData = new SAttachData;
			dataPtrVec.push_back(objData);  			

			objData->init(pte,pEnt);
			objPtMap.insert(pte,i,objData);			

			pEnt->close();
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
		#ifdef DEBUG_TO_PL_PRINT_POINTMAP
		acutPrintf(_T("\nfind it(%f,%f,%f)%d!"),adsPoint[0],adsPoint[1],adsPoint[2],ptIndex);  
		#endif
	}
	else
	{
		#ifdef DEBUG_TO_PL_PRINT_POINTMAP
		acutPrintf(_T("\n not find it(%f,%f,%f)!"),adsPoint[0],adsPoint[1],adsPoint[2]);   
		#endif
	}


	//测试：寻找不重叠，且距离不超过dist的点对;
	vector<pair<void*,void*>> vPoints;
	objPtMap.findPointPairs(0.1,vPoints);  
	objPtMap.printPointPairs(vPoints);      


	//最后，释放分配的内存
	vector<SAttachData*>::iterator itrAttData = dataPtrVec.begin();
	SAttachData* pAttData = NULL;
	for(; itrAttData != dataPtrVec.end(); itrAttData++)
	{
		SAttachData* pAttData = (SAttachData*)(*itrAttData);  
		delete pAttData;
		pAttData = NULL;
	}
	acedSSFree(ss);

	//窗口
	//CAcModuleResourceOverride resOverride;
	//findDistDlg myDialog;
	//myDialog.DoModal();
}