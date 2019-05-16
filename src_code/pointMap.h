/*************************************************************************
Copyright (C), 2017-12-1, shiyuan precision maskmaking. Co., Ltd.
FileName: pointMap.h
Author: 
Version:
Date: //zhg 1.0
Description: // 1.对选中的实体集合建立顶点表，供查询;
             // 2.使用map结构; 以点坐标的x、y为查询key值; x的map嵌套y的map；
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改
		// 者及修改内容简述
eg： Date:	Author: 	Modification:
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
//定义一个误差值； 比较俩double时用;
//.....;

//map比较函数
//知道为啥这么定义比较函数吗？：因为在map内部是这么使用dblcmp的：
//1）测试是否相等：先测试 i是否小于j，不成立，然后测试j是否小于i，不成立； 这样才判定相等；
//2）测试i是否小于j：直接就测试出来了;
//3）测试i是否大于j：如果1）和2）都不成立，就大于了;
struct dblcmp
{
	bool operator()(const double i,const double j) const 
	{
		return (i < j && abs(i - j) > 0.000001);  
	}
};


//定义SYData所挂载数据格式;
//根据需要可以定义不同内容;
struct SAttachData
{
	AcGePoint3d m_pt3d;
	AcDbObjectId m_entId;  //点所属实体ID;       
	void print();  //print itself;       
	void init(const AcGePoint3d& pt,const AcDbEntity* pEnt);    //initialize self;   
};



//key为y坐标
struct SYData
{
	SYData();
	SYData(IN const SYData& src);  //copy constructor;   
	~SYData();
	double m_y;   //y坐标;  
	AcGePoint3d pt;    

	int m_PointIndex;  //点的序号（在图结构中）   
	void* m_dataAttach;  //挂载其他数据; 

private:
	bool isDistSmaller(IN const double dist, IN const vector<double,SYData,dblcmp>::iterator itrB);  
};


//key为x坐标;
//ptIndex:点坐标在图中的序号;
struct SXData
{
	SXData();
	~SXData();
	SXData(IN const SXData& src);  //copy constructor;   
	double m_x;     //x坐标;
	int m_nDotNum; //小数保留多少位;  
	map<double,SYData,dblcmp> m_pPointMap;  //嵌套map;    

	bool find(IN const double yVal, OUT int& ptIndex);  //查找y-key;        

	map<double,SYData,dblcmp>::iterator insert(IN const double yVal,IN const int ptIndex,OUT void* voidPtr = NULL); //插入y key;    
	unsigned int erase(IN const double yVal);  //delete;     

	void print();  //测试，打印信息;  

	//查找和输入坐标点距离小于dist的点;      
	//查找upper_bound及lower_bound        
	map<double,SYData,dblcmp>::iterator syDataBegin();  //返回m_pPointMap遍历器起始指针;     
	map<double,SYData,dblcmp>::iterator syDataEnd();    //返回m_pPointMap遍历器结束指针;     
	bool chkLessDistPoints(IN const double dist,IN const double xcoord,IN const SYData syData, OUT vector<pair<void*,void*>>& vPointPairs);
	bool isDistGreater(IN const double x1,IN const double y1,IN const double x2,IN const double y2,IN const double dist);  
	bool isDigitGreater(IN const double d1,IN const double d2); //比较俩double大小;     
	bool isEqual(IN const double d1, IN const double d2);  //判断俩double是否相等;   
	bool isDistZero(IN const double x1,IN const double y1,IN const double x2,IN const double y2);  

	//查找元素的函数
	//bool first(OUT map<double,SXData,dblcmp>::iterator itrSxdata,OUT map<double,SYData,dblcmp>::iterator itrSydata,OUT SYData& sydata);  
	//bool next();   //获取下一个sydata;    
	//bool last();  //最后一个;    
	//查找近距离点对;    
	//查找小于某个距离的点对;
	bool findPointPairsLessDist(IN const double xc,IN const double yc,OUT const vector<>);   
};  


//用x和y坐标做为key，建立嵌套map，提供插入、查询、删除等功能;     
//希望能提高点坐标查询速度;     
class CPointMap
{
public:
	CPointMap();
	~CPointMap();  

private:
	map<double,SXData,dblcmp> m_mapXcoord;  
	int m_nDotNum;   //小数的位数;
	bool isDotNumSet; //判断小数位数是否已经设置，设置过了就不能再设置了;    
	
public:
	void insert(IN const double x,IN const double y,IN const int ptIndex,IN void* voidPtr = NULL);  //插入    
	void insert(IN const AcGePoint3d pt,IN const int ptIndex,IN void* voidPtr = NULL);      
	bool find(IN const double x,IN const double y,OUT int& ptIndex,OUT void* voidPtr = NULL);   //查找;        
	bool find(IN const AcGePoint3d pt,OUT int& ptIndex,OUT void* voidPtr = NULL);       
	bool erase(IN const double x,IN const double y);  //删除;       
	bool erase(IN const AcGePoint3d pt);       
	bool setDotNum(IN const int nDotNum);           
	void print();    //打印出map的点;   
	//bool printMap();  //打印出map的点;   

	//寻找小于一个距离的点对集合
public:
	//用void* m_dataVoidPtr来扩展附加数据;更有扩展性;       
	//void insert(IN const double xc,IN const double yc,IN void* voidPtr);      
	//bool find(IN const double xc,IN const double yc,OUT void* voidPtr);     
	void findPointPairs(IN const double dist,OUT vector<pair<void*,void*>>& vPointPairs);   //发现距离小于dist的点对;  
	void printPointPairs(IN vector<pair<void*,void*>>& vPointPairs);  //打印点对;  

	//第二版本寻找近距离点对;
public:
	;
private:
	
	bool initPointIterator();	 //初始化遍历器;   
	bool itrNextPoint();   //遍历所有的点;
private:
	map<double,SXData,dblcmp>::iterator m_itrSXData;   
	map<double,SYData,dblcmp>::iterator m_itrSYData;   
	AcGePoint3d m_curPnt;    //当前正在比较的点;        
	double m_curPntXc;    //当前正在比较的点的x坐标;  
	double m_curPntYc;    //当前正在比较的点的y坐标;   
	SAttachData* m_curAttachData; 

public:   
	static double transByDotNum(IN const double xyVal,IN const int nDotNum); //对xy值进行过滤，保留指定位数的小数;    
	static bool isEqual(IN const double d1,IN const double d2,IN const int m_nDotNum); //检查是否相等（在误差范围内);    
};  



//=========================================
//类COptOfPointPairs;
//用来操作点对数据，比如：first，next，last;
//=========================================
class COptOfPointPairs
{
public:
	//COptOfPointPairs();
	COptOfPointPairs();
	COptOfPointPairs(IN vector<pair<void*,void*>>* vPointPairs);
	~COptOfPointPairs();

public:  
	bool inputPointPairs(IN vector<pair<void*,void*>>* vPointPairs);  //输入点对;  
	bool first(OUT AcGePoint3d& pt0,OUT AcGePoint3d& pt1);  
	bool next(OUT AcGePoint3d& pt0,OUT AcGePoint3d& pt1);  
	bool last(OUT AcGePoint3d& pt0,OUT AcGePoint3d& pt1);  

private:
	vector<pair<void*,void*>>::iterator m_itr; 
	vector<pair<void*,void*>>* m_ptPair;
	bool m_isItrInit; //指示迭代器是否已经初始化; 
	bool m_isInitOk; //指示是否有效初始化了：点对是否有效;  
};




//=========================================
//类COptOfTwoPoints;
//用来操作一个点对数据,比如：放大，缩小，连接;
//=========================================
class COptOfTwoPoints
{
public:
	COptOfTwoPoints();
	COptOfTwoPoints(IN const AcGePoint3d& pt0, IN const AcGePoint3d& pt1); 
	~COptOfTwoPoints();

public:
	bool drawLine();
	bool zoomThem();  //在电脑屏幕上放大这2点;
	//bool isEqual(IN const double d1,IN const double d2,IN const int m_nDotNum);
	
private:
	AcGePoint3d m_ptStart;
	AcGePoint3d m_ptEnd;
};



#endif  //ZHOUHUAGANG_20171201_POINTMAP_DOT_H