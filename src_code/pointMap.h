/*************************************************************************
Copyright (C), 2017-12-1, shiyuan procision maskmaking. Co., Ltd.
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

using namespace std;

#define  DEBUG_TO_PL_PRINT
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
//根据需要可以定义不同格式;
struct SAttachData
{
	AcGePoint3d m_pt3d;
	AcDbEntity* m_pEnt;  //点所属实体;  
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
	void* m_dataVoidPtr;  //挂载其他数据;
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

	bool find(IN const double yVal,OUT int& ptIndex);  //查找y-key;     
	map<double,SYData,dblcmp>::iterator insert(IN const double yVal,IN const int ptIndex); //插入y key;    
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
	void insert(IN const double x,IN const double y,IN const int ptIndex);  //插入   
	void insert(IN const AcGePoint3d pt,IN const int ptIndex);      
	bool find(IN const double x,IN const double y,OUT int& ptIndex);   //查找;       
	bool find(IN const AcGePoint3d pt,OUT int& ptIndex);       
	bool erase(IN const double x,IN const double y);  //删除;      
	bool erase(IN const AcGePoint3d pt);      
	bool setDotNum(IN const int nDotNum);           
	void print();    //打印出map的点;   
	//bool printMap();  //打印出map的点;        

public:
	//以下为二期函数，用void* m_dataVoidPtr来扩展附加数据;更有扩展性;     
	void insert(IN const double xc,IN const double yc,IN void* voidPtr);      
	bool find(IN const double xc,IN const double yc,OUT void* voidPtr);     
	void findPointPairs(IN const double dist,OUT vector<pair<void*,void*>>& vPointPairs);   //发现距离小于dist的点对;  
	void printPointPairs(IN vector<pair<void*,void*>>& vPointPairs);  //打印点对;    

public:   
	static double transByDotNum(IN const double xyVal,IN const int nDotNum); //对xy值进行过滤，保留指定位数的小数;    
	static bool isEqual(IN const double dstn,IN const double d2,IN const int m_nDotNum); //检查是否相等（在误差范围内);    
};



#endif  //ZHOUHUAGANG_20171201_POINTMAP_DOT_H