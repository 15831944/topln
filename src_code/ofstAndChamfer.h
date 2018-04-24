/*************************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: ofstAndChamfer.h
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
 dec-13 2017 owal
*************************************************************************/
#ifndef  ZHOUHUAGANG_OFSTANDCHAMFER_H
#define  ZHOUHUAGANG_OFSTANDCHAMFER_H



#include <vector>
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


//命令ofstpl
void ofstPline();
void testIfCCWPline(); //old version,give up;
void testPlineCCW();  

//多义线是否闭合;
enum EIsPlineClosed
{
	REAL_CLOSED,   //真的闭合
	ASSUME_CLOSED,  //起点和终点重合，但是多义线没有closed;
	NOT_CLOSED      //起点和终点不重合;
};

enum EIsPlineCCW
{
	CLOCKWISE_PLINE,
	UNCLOCKWISE_PLINE,
	ERROR_PLINE
};




//针对多义线的动作类
//用法：由用户输入偏移值; 然后自动offset
//只针对闭合多义线及圆实体;
class CPlineOffset
{
public:
	CPlineOffset();
	~CPlineOffset();

private: //member var;   
	double m_offsetDist;  //offset distance;    
	//AcDbVoidPtrArray m_plineArr;  //盛放offset后的polyline;    
public:
	void ofst(IN ads_name ss,OUT AcDbVoidPtrArray& m_plineArr);  //对多义线offset;     
	void ofst(IN AcDbPolyline* pPlineOrg, OUT AcDbVoidPtrArray& m_plineArr);  
	void ofst(IN AcDbCircle* pCircle);
	double setOffsetDist(IN const double dblDist);  //offset value;   
	EIsPlineCCW isPlineCCW(IN AcDbPolyline* pline);  //判断多义线是否逆时针;    
private:	
	bool checkPolyline(IN const AcDbEntity* pEnt);   
	bool checkOffsetRsltSize(IN const AcDbExtents orgExt,IN const AcDbExtents newExt,IN const double dblOfstSize);  
};



#define  VECTOR_TOL (0.000001)
//===========================
//以下方法被证明对凹多边形无效;无效无效无效无效无效无效无效*无效*无效*无效*无效*无效*无效!
//如果是凸多边形，也不需要搞这么复杂;
//===========================
//class CIsPlineCCW
//判断一条多义线是否逆时针；结果只对闭合多义线有效;
//如果多义线不闭合,判断结果是无效的;
class CIsPlineCCW
{
private:
	vector<AcGeVector3d> m_allVec3d; //收集所有vector3d;
	EIsPlineClosed m_isClosed;  //多义线是否闭合;
	AcDbPolyline* m_pline;
	int m_nNumVerts;   //多义线顶点数;
	vector<double> m_crossProductNum;  //每个顶点处都两条边代表都向量的叉积的z值; 
	vector<double> m_anglesLeft;  //m_allAngles整理后的;
	const double m_tol;  //判断向量是否平行的误差值;
public:
	CIsPlineCCW();
	CIsPlineCCW(IN  AcDbPolyline* pPline);
	~CIsPlineCCW();
public:	
	EIsPlineClosed isClosed(IN const AcDbPolyline* pline);
	EIsPlineCCW isPlineCCW(IN AcDbPolyline* pPline);  //interface;

private:
	void calVectorOfVertex(IN const int iIndex); //求某个顶点处的vector3d;
	void getAllVecOfVtx(); //获取所有顶点的vector;
	void getAllAngles();  //获取所有顶点处夹角，逆时针夹角为正，顺时针为负;
	void getFirstAng();  //第一个顶点的夹角要特殊处理;
	//void getLastAng();  //最后一个顶点的夹角要特殊处理; 最后一个不用处理;
	EIsPlineCCW isCCW();  //根据夹角集合判断是否逆时针;
	bool inputPolyline(IN AcDbPolyline* pPline);	
};


//最高点是在端点处，还是在弧段中间;
enum ETopY
{
	TOPY_INIT_STATUS,  //初始化状态;
	TOPY_ON_START,
	TOPY_IN_MIDDLE,
	TOPY_ON_END,
	TOPY_ALL_LINE  //整条线都是最高点;
};

enum ECCW_Pline
{
	ECCW_YES,
	ECCW_NO,
	ECCW_UNKOWN
};


//struct STopY:记载所在顶点序号，弧段类型，最高点类型; 
struct STopY
{
	int m_iIndexVtx;  //顶点序号;
	ETopY m_topYLocation;  //最高点在弧段什么位置;  
	AcGePoint2d m_pt2d;  //点坐标;
	double m_y;  //坐标点的y坐标值;   
	STopY();
	STopY(const STopY& objSrc);
	~STopY();
};


#define TOL_CCW (0.000001)   //6位小数的精度误差值;

//===========================
//class  CJudgeClosedPlineCCW
//判断多义线走线顺序:逆时针还是顺时针
//算法描述：
//寻找最高点及所在边，通过此条边走向判断多义线顺序;  
//===========================
class CJudgePlineCCW
{
private:
	AcDbPolyline* m_pline;
	int m_nNumVerts;  //多义线顶点个数;  

public:
	CJudgePlineCCW(IN AcDbPolyline* pPline);  
	CJudgePlineCCW();
	~CJudgePlineCCW();

public:
	bool inputPline(IN AcDbPolyline* pPline);  
	EIsPlineCCW isPlineCCW();  

private:
	bool prsTopY(OUT struct STopY& sta); //计算多义线某条边的最高点状态; 
	bool prsTopYArcSeg(IN const int iIndexVtx,OUT struct STopY& sta);    
	bool prsTopYLineSeg(IN const int iIndexVtx ,OUT struct STopY& sta);   
	//bool isHigherPtOnArc(AcGeCircArc2d& sGeArc2d);
	EIsPlineCCW whenAllLine(const STopY& sta); //最高点是一条水平线，判断其走向（即顺逆时针）
	EIsPlineCCW whenPtOnStart(const STopY& sta);   //判断顺逆时针;
	EIsPlineCCW whenPtOnEnd(const STopY& sta);    //判断顺逆时针; 
	EIsPlineCCW whenPtOnMiddle(const STopY& sta);   //判断顺逆时针; 
	double calRadian(const STopY& sta);  //计算角度;弧度为单位，范围[0,2Pi];  

	//bool isHigherPtOn(IN const double startAng,IN const double endAng,IN const double dblBug);
};

#endif //ZHOUHUAGANG_OFSTANDCHAMFER_H