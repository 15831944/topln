/*************************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: 
Version:
Date: // hugo、1.0
Description: // 对图形中arc，line，polyline提取: 
Others: // 其它内容的说明
        1.0 此模块作废，只能作为参考；算法不完整，也不完善. 2017-9-7 周华刚;
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改
		// 者及修改内容简述
eg： Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20160612_SELECTSS_DOT_H
#define  ZHOUHUAGANG_20160612_SELECTSS_DOT_H


#include "..\StdAfx.h"
#include <vector>
#include <stack>    //用栈来装载路径(边)；
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
#include <AdAChar.h>

#include  "opt.h"
#include "elapsetime.h"

void reportAllEnt();

using namespace std;
//=======以下为实体选择\操作类=================================================================================
//用户选择实体只有三个选项：ALL/one layer/select；
//选择实体的类，提供各种选择实体的方法；比如，按层，按用户选择，全部图层；
//输出边和顶点，建图；
class  CSelectEnts
{
public:
	CSelectEnts();
	~CSelectEnts();   

	void usrSelect(ads_name ss);  //用户输入选择ALL,ONE LAYER ,SELECT；
	//在用户选择的实体集基础上，按层获取实体集m_selByLayer；每调用一次，自动换层获取实体，直至遍历所有层；
	//返回false表明所有层遍历完了；
	bool getSSAltLayer(ads_name ssAll,ads_name ss_byLayer);  

private:  //私有函数
	void getSSOfOneLayer(ACHAR *layername,ads_name ss);   //获取某层的实体集；
	void getSSOfUserSelect(ads_name ss);   //用户用鼠标在CAD窗口选择；
	void getSSOfAllDrawing(ads_name ss);   //获取所有实体集；
	void filterFrozenLockedOff(ads_name ss);     //过滤掉冻结的图层的实体；//过滤掉不显示的图层的实体；  //过滤掉锁定的图层的实体；
	void filterByLayer(ACHAR* layername);    //对选择集过滤，符合层名的留下；


	void getAllLayerName();  //获取所有层的层名；

private:
	//ads_name m_sel;  //用户选择的实体（锁定的，冻结的，关闭显示的都过滤掉）；  
	//long m_sslen;    //用户选择的实体长度；

	ACHAR* m_charUserInput;   //用户输入的字符A,S，O；

	//用来记录所有层名，
	vector<ACHAR*> m_allLayerName;  //所有层的层名；
	int m_layerNumber;  //遍历所有层名；
	//ads_name m_ssByLayer;       //在用户选择的实体集m_sel基础上，按层选择（过滤）实体集；
	ACHAR* m_layerName;       //m_selByLayer所在层层名；
};


#endif  //ZHOUHUAGANG_20160612_SELECTSS_DOT_H

//=======以上为实体选择类=================================================================================
