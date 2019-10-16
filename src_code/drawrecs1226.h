/*************************************************************************
Copyright (C), 2016-6-12,XuMiJieZi.
FileName: drawrecs1226.cpp
Author: 
Version:
Date: // hugo、1.0
Description: 
某个客户，用wintopo软件将其bmp图片转换成矢量图，cad之dwg格式。
dwg图形里都是小正方形，重叠在一起，没法整理出实际轮廓；此软件
解决这个问题；方法是，将所有小正方形重画一遍;
;
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改
		// 者及修改内容简述
eg： Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_DRAWRECS_DOT_H
#define  ZHOUHUAGANG_DRAWRECS_DOT_H



#include <vector>
#include <stack>  
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
#include "pointMap.h"



using namespace std;

void drawrects();
//CLASS  CDrawRects
class CDrawRects
{
public:
	CDrawRects();
	~CDrawRects();

private:  //data member
	vector<AcGePoint3d> m_allPoints; //所有的点;
	CPointMap m_ptMap; 
	vector<AcDbPolyline*> m_allPlines; //所有多义线;
public:
	void doit();  //executing;
private:
	void collectPoints(ads_name ss);
	void collectPointsOfLine(IN const AcDbEntity* pEnt);
	void collectPointsOfArc(IN const AcDbEntity* pEnt);
	void collectPointsOfPolyline(IN const AcDbEntity* pEnt);
	bool isPointRight(IN const AcGePoint3d pt3d); //测试此顶点是否可以画正方形;
	void drawRects(IN const AcGePoint3d pt3d);
	void drawAllRects();
	void join2db();
};







#endif  //ZHOUHUAGANG_DRAWRECS_DOT_H