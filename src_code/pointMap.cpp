/************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
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
//����ֱ��find��������upper_bound��lower_bound;
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


//���ԣ���ӡ��Ϣ;
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


//��SDataX�в��Һ������������С��dist�ĵ㣻���ɵ�ԣ� 
//����ֵ�����x1��x2���볬��dist������false����������true;  
//ע�⣺���x1 == x2 ��ֻ��Ҫ���²���y�� ����Ӧ��˫�����y��  

bool
SXData::chkLessDistPoints(IN const double dist,IN const double xcoord,IN const SYData syData,OUT vector<pair<void*,void*>>& vPointPairs)
{
	double x1 = 0;   //��������x
	double x2 = 0;   //��������x;
	double y1 = syData.m_y; //��һ��������y;   
	double y2 = 0; //�ڶ�������y;	
	x1 = m_x;
	x2 = xcoord;
	if(abs(x1-x2) > dist) 
	{
		return false;   
	}

	//x1 == x2  ;ֻ����Ѱ�Ҹ����yֵ;    
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
	else  //x1 != x2,���ϼ�����Ѱ��yֵ;    
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
	m_nDotNum = 5;  //����5λС��;
}


//destructor
CPointMap::~CPointMap()
{
}


//����.
//��double����ȡָ��С��λ;
//���أ�������double����;
//���ָ����ǲ���׼�ģ���������ȫ������������ȡ���double������
//����7.0��6.99999��������û����������;
//�����������Ҫ���ã�û����.
double
CPointMap::transByDotNum(IN const double xyVal,IN const int nDotNum)
{
	double baseNum = pow(10.0,nDotNum);
	long lgTotal = floor(xyVal * baseNum);
	double val = lgTotal / baseNum;  //�޼���Ϊָ��λ����С��;
	//double val = (double)lgTotal;
	return val;
}


//����Χ�ڣ��ж�����ֵ�Ƿ����
bool
CPointMap::isEqual(IN const double d1,IN const double d2,IN const int m_nDotNum)
{
	double dTol = 1.0 / (pow(10.0,m_nDotNum));  //	���ֵ;
	double dRes = d1 - d2;
	if(abs(dRes) <= dTol)
	{
		return true;
	}
	return false;
}


//���룺�Ȳ���x-map���ٲ���y-map; 
//����֮ǰ����Ҫ�Ȳ����Ƿ����; 
//*���ǣ���Ϊ�洢����double����key������ҪԤ�Ȳ����Ƿ����Ȼ�����;
//ptIndex: �������;
//���أ�void;
void
CPointMap::insert(IN const double x,IN const double y,IN const int ptIndex)
{
	//double xf = transByDotNum(x,m_nDotNum);
	//double yf = transByDotNum(y,m_nDotNum);
	double xf = x;
	double yf = y;

	//����xֵ;
	map<double,SXData,dblcmp>::iterator itrRtnX;
	bool bFlag = false;
	pair<map<double,SXData,dblcmp>::iterator,bool> pairRtnX; 
	SXData sx;
	sx.m_x = xf;
	pairRtnX = m_mapXcoord.insert(pair<double,SXData>(xf,sx));  
	itrRtnX = pairRtnX.first;

	//����y;
	map<double,SYData,dblcmp>::iterator itrY;
	itrY = itrRtnX->second.insert(yf,ptIndex);  //����insert��SXData�ĳ�Ա����;
}


//insert
void
CPointMap::insert(IN const AcGePoint3d pt,IN const int ptIndex)
{
	insert(pt.x,pt.y,ptIndex);
}


//���ң�����2�Σ���һ�β�x���ڶ��β�y;
//��Ϊkey��double���ͣ����Բ��������������������ң�Ӧ������upper_bound��lower_bound;
//���أ�1.�Ƿ���ҵ���;
bool
CPointMap::find(IN const double x,IN const double y,OUT int& ptIndex)
{
	//double xc = transByDotNum(x,m_nDotNum);  //�޼���Ϊָ��λ����С��;
	//double yc = transByDotNum(y,m_nDotNum);
	double xc = x ;
	double yc = y ;	

	//acutPrintf(_T("\n(%15f,%15f)"),xc,yc);

	map<double,SXData,dblcmp>::iterator itr =  m_mapXcoord.find(xc); //
	if(itr != m_mapXcoord.end())  //��x���в���.  
	{		
		if(itr->second.find(yc,ptIndex))
		{
			return true;  //����ҵ��ͷ���; 
		}
	}
	return false;  //���е�����˵��û�ҵ�.
}


//find
bool
CPointMap::find(IN const AcGePoint3d pt,OUT int& ptIndex)
{
	return find(pt.x,pt.y,ptIndex);
}


//set dot number
//���أ�true˵�����óɹ��� false˵������ʧ��; ʧ��ԭ�����벻��ȷ���Ѿ����ù���;
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


//Ѱ�Ҿ���С��dist�ĵ��
void
CPointMap::findPointPairs(IN const double dist,OUT vector<pair<void*,void*>> pointPairs)
{
	map<double,SXData,dblcmp>::iterator itrxFirst = m_mapXcoord.begin(); //ȡ����x�У������Աȣ�
	map<double,SXData,dblcmp>::iterator itrxNext;    //ȡ����x+n�У��������Ա�; x + n > x;
	map<double,SYData,dblcmp>::iterator itry;  //ȡ��y����;
	
	//three level loops;
	//ȡ��x����;
	double mx = 0;
	double my = 0;
	SYData syData;
	void* voidDataPtr = NULL;
	bool flag = false;  //�ж��Ƿ��ü���������һ��sxdata; 
	for(; itrxFirst != m_mapXcoord.end(); itrxFirst++) 
	{
		mx = itrxFirst->second->m_x;
		//����ȡ��y���꼰��������;
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
				if(!flag)  //����false��˵��x1��x2����̫Զ�ˣ������˳�����ѭ����;
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
	objPtMap.print();  //��ӡ������;

	//����Ѱ��һ����;
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