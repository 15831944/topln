// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"

#include ".\src_code\to_pl.h"
#include ".\src_code\basefunc.h"
#include ".\src_code\reportEntitiesNumber.h"
#include ".\src_code\pointMap.h" 
#include ".\src_code\ofstAndChamfer.h" 
#include ".\src_code\drawrecs1226.h"
#include ".\src_code\createlayer.h"
#include ".\src_code\draw2.h"
#include ".\src_code\purge.h" 
#include ".\src_code\selfintersect.h" 
#include ".\src_code\explode_insert.h" 
#include ".\src_code\findDist.h" 
#include ".\src_code\shanghaisanxing.h" 
#include ".\src_code\kubun.h" 


//-----------------------------------------------------------------------------
#define szRDS _RXST("owal")

void Test();

struct def_cmdfun
{
	CString cmdName_;
	AcRxFunctionPtr funAddr_;
	Adesk::Int32 commandFlags_;
};


def_cmdfun cmdFunTbl[] = 
{
	{_T("k"), displayAllLayers, ACRX_CMD_MODAL},  //显示所有层；
	{_T("brkint"), addVtxAtIntersetPoints, ACRX_CMD_MODAL},  //在交点处打断多义线（添加新顶点）;
	{_T("g"), closeAllLayers, ACRX_CMD_MODAL},  //关闭显示所有层；
	{_T("test"), Test, ACRX_CMD_MODAL},  //测试使用
	{_T("topln"), to_polylines, ACRX_CMD_MODAL},   //测试使用
	{_T("ext"), test_extentes, ACRX_CMD_MODAL},  //测试使用
	{_T("repo"), reportAllEnt, ACRX_CMD_MODAL},   //report all entities' type and number;
	{_T("testmap"), testPointMapClass, ACRX_CMD_MODAL},   //test point map 
	{_T("xgap"), mendGapBtwPointPairDlg, ACRX_CMD_MODAL},   //调用对话框，对话框调用point-pair gap修补功能;
	{_T("ofstpl"), ofstPline, ACRX_CMD_MODAL},   //offset the polyline closed; 
	{_T("plccw"), testIfCCWPline, ACRX_CMD_MODAL},   //test polyline ccw,givn up.   
	{_T("isplccw"), testPlineCCW, ACRX_CMD_MODAL},   //test polyline ccw,new,effect;
	{_T("drects"), drawrects, ACRX_CMD_MODAL},   //draw rects;  特别针对从wintop软件出来的小方格；wintop：完成jpg到矢量图形转换;
	{_T("ra"), createNewLayer, ACRX_CMD_MODAL},   //create new layer;   
	{_T("owal1"), laigerTwo, ACRX_CMD_MODAL},   //画0和1的长方形;   
	{_T("pc"), purge, ACRX_CMD_MODAL},   //清理图形:去除无用层、5s;  
	{_T("xi"), xb, ACRX_CMD_MODAL},   //分解minsert，insert实体;  2018-4-28-sat github test aaas
	{_T("test001"), testCOptOfTwoPnts::testCOptOfTwoPoints, ACRX_CMD_MODAL},   //
	{_T("sanxing"), sanxing, ACRX_CMD_MODAL},   //针对三星客户图形不同颜色line加宽不同宽度的半自动化;
	{_T("kubun"), kubun, ACRX_CMD_MODAL}   //针对三星客户图形不同颜色line加宽不同宽度的半自动化;

};

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CArxProject1App : public AcRxArxApp {

public:
	CArxProject1App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		for (int i = 0; i < ARRAYSIZE(cmdFunTbl); ++i)
		{
			acedRegCmds->addCommand(_T("Owal"), cmdFunTbl[i].cmdName_,cmdFunTbl[i].cmdName_, cmdFunTbl[i].commandFlags_, cmdFunTbl[i].funAddr_);
		}

		return (retCode) ;
	}	

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		acedRegCmds->removeGroup(_T("Owal"));

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;



void 
Test()
{
	acutPrintf(_T("hello,arx!\n"));

	//测试三维向量叉积----------------------------------start
	AcGeVector3d vec1(1,0,0);
	AcGeVector3d vec2(1,1,0);
	AcGeVector3d vec3;
	vec3 = vec1.crossProduct(vec2);

	AcGeVector3d vec4;
	vec4 = vec2.crossProduct(vec1);

	double dblLen = vec3.length();
	acutPrintf(_T("\n vec3 length is %f,%f,%f,%f"),dblLen,vec3.x,vec3.y,vec3.z);

	dblLen = vec4.length();
	acutPrintf(_T("\n vec4 length is %f,%f,%f,%f"),dblLen,vec4.x,vec4.y,vec4.z);
	//测试三维向量叉积----------------------------------end
}

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArxProject1App)

