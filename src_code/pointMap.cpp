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
SXData::insert(IN const double yVal,IN const int ptInex,OUT void* voidPtr)   
{
	pair<map<double,SYData,dblcmp>::iterator,bool> pairRtn;
	map<double,SYData,dblcmp>::iterator itrY;
	bool bflag = false;
	SYData stY;
	stY.m_y = yVal;    
	stY.pt.set(m_x,yVal,0);
	stY.m_PointIndex = ptInex;
	stY.m_dataAttach = voidPtr;  //��������;
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


//���ԣ���ӡmap�����е����Ϣ;
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


//����������룬�������������Ƚ�
//��������ڣ���Ҫ�ų���ȵ����;
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


//�Ƚ���double��С;  
//d1����d2������true;
//���ֵ���ã�
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


//�Ƚ���double��С;
//d1����d2������true; 
//���ֵ���ã�
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



//����m_pPointMap��������ʼָ��;
map<double,SYData,dblcmp>::iterator 
SXData::syDataBegin()
{
	return m_pPointMap.begin();   
}


 //����m_pPointMap����������ָ��;
map<double,SYData,dblcmp>::iterator 
SXData::syDataEnd()
{
	return m_pPointMap.end();
}


//�ж���ʵ���Ƿ����
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


//��SDataX�в��Һ������������С��dist�ĵ㣻���ɵ�ԣ� 
//����ֵ�����x1��x2���볬��dist������false�����������������0������false�� ��������true;  
//ע�⣺���x1 == x2 ��ֻ��Ҫ���²���y�� ����Ӧ��˫�����y��  
bool
SXData::chkLessDistPoints(IN const double dist,IN const double xcoord,IN const SYData syData,OUT vector<pair<void*,void*>>& vPointPairs)
{
	double x1 = 0;   //�����룩Դ�����x�� 
	double x2 = 0;   //Ŀ�������x;  
	double y1 = syData.m_y; //�����룩Դ�����y;       
	double y2 = 0; //Ŀ�������y;	
	double ytemp = y1; //temp y;     
	x1 = xcoord;   
	x2 = m_x;  
	if(abs(x1-x2) > dist)     //x��������Ѿ�����dist; 
	{
		return false;     
	}

	//x1 == x2  ;ֻ����Ѱ�Ҹ����yֵ;    
	map<double,SYData,dblcmp>::iterator itrYc; // = m_pPointMap.begin();	 
	if(isEqual(x1,x2))    
	{
		itrYc = m_pPointMap.lower_bound(ytemp);     
		while(itrYc != m_pPointMap.end()) 
		{			      
			y2 = itrYc->second.m_y;  		
#ifdef DEBUG_TO_PL_PRINT_POINTMAP
			acutPrintf(_T("\n �Ƚϵ�ԣ�"));
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
	else  //x1 != x2,���ϼ�����Ѱ��yֵ;    
	{
		//��������:����©��begin�������Ԫ��;
		itrYc = m_pPointMap.lower_bound(ytemp); 
		bool isBreak = false;
		//��Ϊ����itrYc��yֵ��not less than y1,���������ϣ�yֵ��С�ķ�����һ��;
		//itrYc--;
		while(true)   //�п���itrYcֱ�ӵ���end;
		{	
			//if(itrYc == m_pPointMap.end()) //������itrYcͣ����end���������;  
			//{
			//	itrYc--;
			//}
			if(itrYc == m_pPointMap.begin())
			{
				isBreak = true; //ִ������־��˳�ѭ��;  
			}
			else  //including the end;
			{
				itrYc--;
			}

			y2 = itrYc->second.m_y;      // ���ԣ������������ x1=0��y1=0.001�� x2=0.001��y2Ӧ�õ���0;
#ifdef DEBUG_TO_PL_PRINT_POINTMAP
			acutPrintf(_T("\n �Ƚϵ�ԣ�"));
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

		//��������; 
		itrYc = m_pPointMap.lower_bound(ytemp);		
		//if(itrYc != m_pPointMap.end())
		//{
		//	itrYc++;//��Ԫ���Ǻ����ϲ����ص���;����Ҫ�޳�;
		//}
		while(itrYc != m_pPointMap.end())  
		{			  
			y2 = itrYc->second.m_y;
#ifdef DEBUG_TO_PL_PRINT_POINTMAP
			acutPrintf(_T("\n �Ƚϵ�ԣ�"));
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

	return true;  //����x2-x1>dist,�����������true; 
}



//Ѱ��С��dist����ĵ��
//vPointPairs:װ�ص������;
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


//����yֵ����Ѱ��С��dist����ĵ㣻
//����ֵ��true��ʾ�ҵ���һЩ����Ҫ��ĵ㣻 false��ʾû���ҵ�����Ҫ��ĵ�;
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
//��ʼ�������߸�ֵ��
void
SAttachData::init(const AcGePoint3d& pt,const AcDbEntity* pEnt)
{
	m_pt3d = pt;
	m_entId = pEnt->objectId();
}


//��ӡ��Ϣ;
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
CPointMap::insert(IN const double x,IN const double y,IN const int ptIndex,IN  void* voidPtr)
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
	itrY = itrRtnX->second.insert(yf,ptIndex,voidPtr);  //����insert��SXData�ĳ�Ա����;
}


//insert
void
CPointMap::insert(IN const AcGePoint3d pt,IN const int ptIndex,IN void* voidPtr)
{
	insert(pt.x,pt.y,ptIndex,voidPtr);   
}


//ɾ��һ��Ԫ��
//������ɾ�������ҵ�x��Ȼ��ɾ��y��Ȼ���ж�x�Ƿ�Ϊ�գ��վ�ɾ��x;
//����ֵ��true-ɾ���ɹ��� false-ɾ�����ɹ�,���ܸ�Ԫ�ز�����;
bool
CPointMap::erase(IN const double x,IN const double y)
{
	//if(m_mapXcoord.empty())
	//{		
	//	return false;  //�յ�;
	//}

	//find SXData
	map<double,SXData,dblcmp>::iterator itrX; // = m_mapXcoord.begin();	
	itrX = m_mapXcoord.find(x);
	if(itrX == m_mapXcoord.end())  
	{
		return false;  //û�ҵ�����ɾ��������false��	
	}

	//��y;
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


//ɾ��ĳ��Ԫ��
bool
CPointMap::erase(IN const AcGePoint3d pt)
{
	return erase(pt.x,pt.y);
}


//ptIndex:������;
//���ң�����2�Σ���һ�β�x���ڶ��β�y;
//��Ϊkey��double���ͣ����Բ��������������������ң�Ӧ������upper_bound��lower_bound;-�ڲ�ʵ��;
//���أ�true-�ҵ���;false-û�ҵ�;
bool
CPointMap::find(IN const double x,IN const double y,OUT int& ptIndex,OUT void* voidPtr)
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
CPointMap::find(IN const AcGePoint3d pt,OUT int& ptIndex,OUT void* voidPtr)
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
#ifdef DEBUG_TO_PL_PRINT_POINTMAP	 
	map<double,SXData,dblcmp>::iterator itr = m_mapXcoord.begin();
	for(; itr != m_mapXcoord.end(); itr++)
	{
		itr->second.print();
	}
#endif
}


//Ѱ�Ҿ���С��dist�ĵ��;  
//�����е�������Ѱ��; ������������㼯��;   
//����ѭ����
void
CPointMap::findPointPairs(IN const double dist,OUT vector<pair<void*,void*>>& pointPairs)     
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
		//ȡ��x����;
		mx = itrxFirst->second.m_x;   
		
		itry = itrxFirst->second.syDataBegin();
		for(; itry != itrxFirst->second.syDataEnd(); itry++)      
		{
			//����ȡ��y���꼰��������;
			syData = (SYData)(itry->second);    
			my = itry->second.m_y;   
			voidDataPtr = itry->second.m_dataAttach;     

			//third loop���Ƚ�ÿһ��x��;
			for(itrxNext = itrxFirst; itrxNext != m_mapXcoord.end();itrxNext++)  //
			{
				flag = itrxNext->second.chkLessDistPoints(dist,mx,syData,pointPairs);      
				if(!flag)  //����false��˵��x1��x2����̫Զ�ˣ������˳�����ѭ����; 
				{
					break;     
				}
			}
		}
	}
}


//��ӡ��ȡ�ĵ��
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



//m_itrSXDataFirst����һ��sxData������ȡ��һ����;
//m_itrSXDataNext: �ڶ���sxData������Ѱ�Һ͵�һ���������ĵڶ�����;
//m_itrSYDataFirst: ��m_itrSXDataFirst��ȡ���ĵ�syData;
//���ܣ��ӵ㼯��Ѱ�Ҿ���С��dist�ĵ��;
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
				else  //m_itrSXDataNext��m_itrSXDataFirst��x���볬��dist��; 
				{
					break;
				}
			}
		}
	}
}


//=========================================
//��COptOfPointPairs;
//��������������ݣ����磺first��next��last;
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
	//����Ϊ���Դ���:����m_ptPair�Ƿ���Ч;
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
	//����Ϊ���Դ���;

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
	//���жϵ������Ƿ��ʼ��;
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
//��COptOfTwoPoints;
//��������һ���������,���磺�Ŵ���С������;
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


//�������㻭��;
bool
COptOfTwoPoints::drawLine()
{
	AcDbLine* objLine = new AcDbLine(m_ptStart,m_ptEnd); 
	join2database(objLine);
	return true;
}


//��������Ϊ��Χ�Ŵ���Ұ
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
//ִ�к�������Ӧ���test001;
//������pointMap����;
//========================================= 
//����COptOfTwoPoints
void testCOptOfTwoPnts::testCOptOfTwoPoints()
{
	AcGePoint3d pt1(1.0,1.0,0);
	AcGePoint3d pt2(5,5,0);
	COptOfTwoPoints objOpt(pt1,pt2);
	objOpt.drawLine();
}	



//=========================================
//ִ�к�������Ӧ���testmap;
//Ҳ�ǲ�����pointMap����;
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
	SAttachData* objData;  //��������;
	vector<SAttachData*> dataPtrVec; //�ռ�֮�������ͷŶ�̬������ڴ�; 
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


	//���ԣ�Ѱ�Ҳ��ص����Ҿ��벻����dist�ĵ��;
	vector<pair<void*,void*>> vPoints;
	objPtMap.findPointPairs(0.1,vPoints);  
	objPtMap.printPointPairs(vPoints);      


	//����ͷŷ�����ڴ�
	vector<SAttachData*>::iterator itrAttData = dataPtrVec.begin();
	SAttachData* pAttData = NULL;
	for(; itrAttData != dataPtrVec.end(); itrAttData++)
	{
		SAttachData* pAttData = (SAttachData*)(*itrAttData);  
		delete pAttData;
		pAttData = NULL;
	}
	acedSSFree(ss);

	//����
	//CAcModuleResourceOverride resOverride;
	//findDistDlg myDialog;
	//myDialog.DoModal();
}