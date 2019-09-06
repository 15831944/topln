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

#include <vector>
#include <list>
#include <map>


//建立3个结构：
//1.存储所有线段的端点list（包括新发现的交点;
//2.存储扫描线上的点set（二叉树？）；
//3.存储发现的交点;

//几个问题点：
//1.端点重合的线段怎么表达;
//2.圆弧怎么表示成线段？: 弧段用通过中心的垂线和水平线划分成四段; 每段弧是单调的;
//3.交点是下端点；

//本软件功能：求N个线段的交点集;
//


/*
结构名：
用途：表示一个线段，可能是一条直线，也可能是一条弧;   
*/
struct structSegement
{
	;
};


#include "segments_intersections.h"
