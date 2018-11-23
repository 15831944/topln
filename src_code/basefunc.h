#ifndef  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SELECTSELECTIONSET_H
#define  OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SELECTSELECTIONSET_H


//#include "afxwin.h"
//#include "stdafx.h"
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

#include <string>
#include <iostream>
//#include <afx.h>
#include "atlstr.h"

#include "elapsetime.h"

using namespace std;
/*
 *  developer's function
*/

enum 
BUILDLAYER
{
	THE_LAYER_EXISTED,
	THE_LAYER_NOT_EXISTED,
	SOMETHING_TROUBLE
};


bool select(ads_name ss);
//void makeLayer(char *layername);
BUILDLAYER addLayer(const ACHAR* layername);
void setCurrentLayer(const ACHAR *layername);
void upstring(ACHAR *pName);
AcDbObjectId join2database(AcDbEntity *pEnt);
void  setlayer(AcDbEntity *pent,ACHAR* layername);
void setlayer(ads_name ss_unit,ACHAR* layername);
void  numLayers(int& numLayers);//统计数据库有多少层;
bool isameLayer(ads_name ss_old,ads_name ss_new);
void to_upper();	//

wstring getErpID();	//从用户取得流程单号.
wstring getTodayData();	//取得电脑当天日期.

void getnet();
void test_extentes();


//====================================
//class  CExt
//计算所选实体集合的最外框线，框线宽度，长度级别;
//用途：估量图形坐标数值级别，是100，1000，10000mm级别？
//从而设置合理的AcGeTol值;
// 输入：实体集合;
// 输出：最外框线坐标、宽度，长度;
//====================================
//获取实体集的extent;
//用途之一：大概测算图形比例，确定大概坐标误差值;例如：
//bool isEqualTo(
//			   const AcGePoint3d& pnt, 
//			   const AcGeTol& tol = AcGeContext::gTol
//			   ) const;
class CExtentOfSS
{
public:
	CExtentOfSS();
	~CExtentOfSS();
private:
	double m_xLen;
	double m_yLen; //y方向总长;
	double m_minX;
	double m_minY;
	double m_maxX;
	double m_maxY; //最大y值;
	AcGePoint3d m_minPoint;   
	AcGePoint3d m_maxPoint;    
public:
	bool getExtents(IN ads_name ss);
	bool getExtents(IN const AcDbEntity* pEnt,OUT AcDbExtents& objExt);  
	bool getExtents(IN ads_name ss,OUT AcDbExtents& objExt);  
	bool getExtents(IN const AcDbVoidPtrArray pEntArr,OUT AcDbExtents& objExt);  
	double getLenX(IN double& x);
	double getLenY(IN double& y);
	AcGePoint3d getMinPoint(OUT AcGePoint3d& pt3d);   
	AcGePoint3d getMaxPoint(OUT AcGePoint3d& pt3d);   
	static bool isExtEffect(IN const AcDbExtents& objExt);  
};



class CSomePublicFun
{
	//CSomePublicFun();
	//~CSomePublicFun();
public:
	bool isCstringDigit(IN const CString &szStr); 
	bool getArcEndPoints(const AcDbArc* arcptr,OUT AcGePoint3d& pts,OUT AcGePoint3d& pte);
};






#endif  //OWAL_PC2008_THIRTY_TWO_YEARS_OLD_SELECTSELECTIONSET_H