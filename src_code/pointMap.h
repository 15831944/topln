/*************************************************************************
Copyright (C), 2017-12-1, shiyuan precision maskmaking. Co., Ltd.
FileName: pointMap.h
Author: 
Version:
Date: //zhg 1.0
Description: // 1.��ѡ�е�ʵ�弯�Ͻ������������ѯ;
             // 2.ʹ��map�ṹ; �Ե������x��yΪ��ѯkeyֵ; x��mapǶ��y��map��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
eg�� Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20171201_POINTMAP_DOT_H
#define  ZHOUHUAGANG_20171201_POINTMAP_DOT_H



#include <vector>
#include <map>   
#include <algorithm>


#include <stdlib.h>
#include <ctype.h>
#include <aced.h>
#include <rxregsvc.h>
#include <adscodes.h>
#include <dbsymtb.h>
#include <dblayout.h>
#include <dbapserv.h>
#include <acdblmgr.h>
#include <dbents.h>
#include <dbpl.h>
#include <dbid.h>
#include <acutads.h>
#include <string.h>
#include <stdio.h>
#include <acedads.h>
#include <dbmain.h>
#include <geplin2d.h>
#include <math.h>
#include <acarray.h>
#include <dbdict.h>
#include <dbgroup.h>
#include <rxobject.h>
#include <gelnsg2d.h>
#include <gevec2d.h>

#include  "opt.h"
#include "elapsetime.h"
#include "basefunc.h"
//#include "findDist.h"

using namespace std;

namespace testCOptOfTwoPnts
{
	void testCOptOfTwoPoints();
}
//void testOptOfTwoPoints();


#define  DEBUG_TO_PL_PRINT_POINTMAP
void testPointMapClass();
//����һ�����ֵ�� �Ƚ���doubleʱ��;
//.....;

//map�ȽϺ���
//֪��Ϊɶ��ô����ȽϺ����𣿣���Ϊ��map�ڲ�����ôʹ��dblcmp�ģ�
//1�������Ƿ���ȣ��Ȳ��� i�Ƿ�С��j����������Ȼ�����j�Ƿ�С��i���������� �������ж���ȣ�
//2������i�Ƿ�С��j��ֱ�ӾͲ��Գ�����;
//3������i�Ƿ����j�����1����2�������������ʹ�����;
struct dblcmp
{
	bool operator()(const double i,const double j) const 
	{
		return (i < j && abs(i - j) > 0.000001);  
	}
};


//����SYData���������ݸ�ʽ;
//������Ҫ���Զ��岻ͬ����;
struct SAttachData
{
	AcGePoint3d m_pt3d;
	AcDbObjectId m_entId;  //������ʵ��ID;       
	void print();  //print itself;       
	void init(const AcGePoint3d& pt,const AcDbEntity* pEnt);    //initialize self;   
};



//keyΪy����
struct SYData
{
	SYData();
	SYData(IN const SYData& src);  //copy constructor;   
	~SYData();
	double m_y;   //y����;  
	AcGePoint3d pt;    

	int m_PointIndex;  //�����ţ���ͼ�ṹ�У�   
	void* m_dataAttach;  //������������; 

private:
	bool isDistSmaller(IN const double dist, IN const vector<double,SYData,dblcmp>::iterator itrB);  
};


//keyΪx����;
//ptIndex:��������ͼ�е����;
struct SXData
{
	SXData();
	~SXData();
	SXData(IN const SXData& src);  //copy constructor;   
	double m_x;     //x����;
	int m_nDotNum; //С����������λ;  
	map<double,SYData,dblcmp> m_pPointMap;  //Ƕ��map;    

	bool find(IN const double yVal, OUT int& ptIndex);  //����y-key;        

	map<double,SYData,dblcmp>::iterator insert(IN const double yVal,IN const int ptIndex,OUT void* voidPtr = NULL); //����y key;    
	unsigned int erase(IN const double yVal);  //delete;     

	void print();  //���ԣ���ӡ��Ϣ;  

	//���Һ�������������С��dist�ĵ�;      
	//����upper_bound��lower_bound        
	map<double,SYData,dblcmp>::iterator syDataBegin();  //����m_pPointMap��������ʼָ��;     
	map<double,SYData,dblcmp>::iterator syDataEnd();    //����m_pPointMap����������ָ��;     
	bool chkLessDistPoints(IN const double dist,IN const double xcoord,IN const SYData syData, OUT vector<pair<void*,void*>>& vPointPairs);
	bool isDistGreater(IN const double x1,IN const double y1,IN const double x2,IN const double y2,IN const double dist);  
	bool isDigitGreater(IN const double d1,IN const double d2); //�Ƚ���double��С;     
	bool isEqual(IN const double d1, IN const double d2);  //�ж���double�Ƿ����;   
	bool isDistZero(IN const double x1,IN const double y1,IN const double x2,IN const double y2);  

	//����Ԫ�صĺ���
	//bool first(OUT map<double,SXData,dblcmp>::iterator itrSxdata,OUT map<double,SYData,dblcmp>::iterator itrSydata,OUT SYData& sydata);  
	//bool next();   //��ȡ��һ��sydata;    
	//bool last();  //���һ��;    
	//���ҽ�������;    
	//����С��ĳ������ĵ��;
	bool findPointPairsLessDist(IN const double xc,IN const double yc,OUT const vector<>);   
};  


//��x��y������Ϊkey������Ƕ��map���ṩ���롢��ѯ��ɾ���ȹ���;     
//ϣ������ߵ������ѯ�ٶ�;     
class CPointMap
{
public:
	CPointMap();
	~CPointMap();  

private:
	map<double,SXData,dblcmp> m_mapXcoord;  
	int m_nDotNum;   //С����λ��;
	bool isDotNumSet; //�ж�С��λ���Ƿ��Ѿ����ã����ù��˾Ͳ�����������;    
	
public:
	void insert(IN const double x,IN const double y,IN const int ptIndex,IN void* voidPtr = NULL);  //����    
	void insert(IN const AcGePoint3d pt,IN const int ptIndex,IN void* voidPtr = NULL);      
	bool find(IN const double x,IN const double y,OUT int& ptIndex,OUT void* voidPtr = NULL);   //����;        
	bool find(IN const AcGePoint3d pt,OUT int& ptIndex,OUT void* voidPtr = NULL);       
	bool erase(IN const double x,IN const double y);  //ɾ��;       
	bool erase(IN const AcGePoint3d pt);       
	bool setDotNum(IN const int nDotNum);           
	void print();    //��ӡ��map�ĵ�;   
	//bool printMap();  //��ӡ��map�ĵ�;   

	//Ѱ��С��һ������ĵ�Լ���
public:
	//��void* m_dataVoidPtr����չ��������;������չ��;       
	//void insert(IN const double xc,IN const double yc,IN void* voidPtr);      
	//bool find(IN const double xc,IN const double yc,OUT void* voidPtr);     
	void findPointPairs(IN const double dist,OUT vector<pair<void*,void*>>& vPointPairs);   //���־���С��dist�ĵ��;  
	void printPointPairs(IN vector<pair<void*,void*>>& vPointPairs);  //��ӡ���;  

	//�ڶ��汾Ѱ�ҽ�������;
public:
	;
private:
	
	bool initPointIterator();	 //��ʼ��������;   
	bool itrNextPoint();   //�������еĵ�;
private:
	map<double,SXData,dblcmp>::iterator m_itrSXData;   
	map<double,SYData,dblcmp>::iterator m_itrSYData;   
	AcGePoint3d m_curPnt;    //��ǰ���ڱȽϵĵ�;        
	double m_curPntXc;    //��ǰ���ڱȽϵĵ��x����;  
	double m_curPntYc;    //��ǰ���ڱȽϵĵ��y����;   
	SAttachData* m_curAttachData; 

public:   
	static double transByDotNum(IN const double xyVal,IN const int nDotNum); //��xyֵ���й��ˣ�����ָ��λ����С��;    
	static bool isEqual(IN const double d1,IN const double d2,IN const int m_nDotNum); //����Ƿ���ȣ�����Χ��);    
};  



//=========================================
//��COptOfPointPairs;
//��������������ݣ����磺first��next��last;
//=========================================
class COptOfPointPairs
{
public:
	//COptOfPointPairs();
	COptOfPointPairs();
	COptOfPointPairs(IN vector<pair<void*,void*>>* vPointPairs);
	~COptOfPointPairs();

public:  
	bool inputPointPairs(IN vector<pair<void*,void*>>* vPointPairs);  //������;  
	bool first(OUT AcGePoint3d& pt0,OUT AcGePoint3d& pt1);  
	bool next(OUT AcGePoint3d& pt0,OUT AcGePoint3d& pt1);  
	bool last(OUT AcGePoint3d& pt0,OUT AcGePoint3d& pt1);  

private:
	vector<pair<void*,void*>>::iterator m_itr; 
	vector<pair<void*,void*>>* m_ptPair;
	bool m_isItrInit; //ָʾ�������Ƿ��Ѿ���ʼ��; 
	bool m_isInitOk; //ָʾ�Ƿ���Ч��ʼ���ˣ�����Ƿ���Ч;  
};




//=========================================
//��COptOfTwoPoints;
//��������һ���������,���磺�Ŵ���С������;
//=========================================
class COptOfTwoPoints
{
public:
	COptOfTwoPoints();
	COptOfTwoPoints(IN const AcGePoint3d& pt0, IN const AcGePoint3d& pt1); 
	~COptOfTwoPoints();

public:
	bool drawLine();
	bool zoomThem();  //�ڵ�����Ļ�ϷŴ���2��;
	//bool isEqual(IN const double d1,IN const double d2,IN const int m_nDotNum);
	
private:
	AcGePoint3d m_ptStart;
	AcGePoint3d m_ptEnd;
};



#endif  //ZHOUHUAGANG_20171201_POINTMAP_DOT_H