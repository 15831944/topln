/*************************************************************************
Copyright (C), 2016-6-12, XuMiJieZi procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: 
Version:
Date: // hugo、1.0
Description: // 对图形中arc，line，polyline提取
Others: // 其它内容的说明
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改
		// 者及修改内容简述
eg： Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20160612_REPORT_ENTITIES_NUMBER_DOT_H
#define  ZHOUHUAGANG_20160612_REPORT_ENTITIES_NUMBER_DOT_H


#include "..\StdAfx.h"

#include <vector>
#include <stack>    //用栈来装载路径(边)；
#include <string>
#include <iostream>
#include <sstream>


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

#include "elapsetime.h"
#include "selectss.h"

void  reportAllEnt();

//#pragma comment("rxapi.lib")

using namespace std;



//每个实体对应一个SData；
//最后每种实体数据也统计在这个结构里；
struct SData
{
	wstring m_className;
	int m_number;	         //隐含表示闭合多义线数量；
	bool isPolylineClosed;	//多一个功能，识别多义线是否闭合；
	int m_numOfNotClosed;  //未闭合的多义线数量；

	SData();
};


class CDataArray 
{
public:
	CDataArray();
	~CDataArray();

private:
	vector<SData> m_nameNumber;   
	wstring m_info;   //最终报告信息;

public:
	bool findAndUpdate(SData* psd); //不存在就加入，存在就增加计数;
	
private:
	void push_back(SData sdat);

public:	
	void printData();
	wstring printInfo();

};



//报告实体的类型，数量；
class CEntitiesReport
{
public:
	CEntitiesReport();
	~CEntitiesReport();

private:
	ads_name ssAll;
	long m_sslength;
	SData m_sdt;  
	CDataArray m_typeAndNumber; //实体类名及数量存放于此；

public:	
	void analysisEntity(ads_name ss); //分析统计实体；
};





#endif  //ZHOUHUAGANG_20160612_REPORT_ENTITIES_NUMBER_DOT_H