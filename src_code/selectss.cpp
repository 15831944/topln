/************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: selectss.cpp
Author: Version :
Date:
Description:  1、line,arc轮廓提取;

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
#include  "to_pl.h"
#include "TCHAR.h"
#include "selectss.h"

//=============用户选择实体类========================================start=======

//用户选择实体；封装类；
CSelectEnts::CSelectEnts()
{
	//获取用户选择;
	m_charUserInput = new ACHAR[133];

	int flag;
	flag = acedGetString(0,_T("Select/One layer/<All layer>:"),m_charUserInput);

	//获取全部图层层名；
	getAllLayerName();
}


//提示用户选择drawing中的图形；
void
CSelectEnts::usrSelect(ads_name ss)
{
	switch(m_charUserInput[0])
	{
	case 'O':getSSOfOneLayer(m_charUserInput,ss);break;
	case 'o':getSSOfOneLayer(m_charUserInput,ss);break;
	case 'S':getSSOfUserSelect(ss);break;
	case 's':getSSOfUserSelect(ss);break;
	case 'a':getSSOfAllDrawing(ss);break;
	case 'A':getSSOfAllDrawing(ss);break;
	default:break;
	}
	//acedSSGet();
	m_layerNumber = -1;  //初始化为-1；
}


//析构函数；
CSelectEnts::~CSelectEnts()
{
	//释放层名申请的内存；
	ACHAR* pstr;
	int sizeAchar = m_allLayerName.size();
	for(int i  = 0 ; i < sizeAchar; i++)
	{
		pstr = m_allLayerName[i];
		delete[] pstr;
	}

	//释放实体集；
}



//用户自己选择实体；
void
CSelectEnts::getSSOfUserSelect(ads_name ss)
{
	int flag;
	flag = acedSSGet(NULL,NULL,NULL,NULL,ss);
	if(flag == RTCAN) 
	{
		return;
	}

	filterFrozenLockedOff(ss); //过滤；

	long sslen;
	if(ss)
	{
		acedSSLength(ss,&sslen);
		acutPrintf(_T("\n有%ld个物体被选中"),sslen);
		//return true;
	}
}


//根据层名，获取该层实体集；
void
CSelectEnts::getSSOfOneLayer(ACHAR* layername,ads_name ss)
{
	int flag;
	long sslen;

	AcDbLayerTable* lytblPtr;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytblPtr, AcDb::kForRead);   

	AcDbLayerTableRecord *lytblrcdPtr;
	lytblPtr->getAt(layername,lytblrcdPtr,AcDb::kForRead);						
	if(lytblrcdPtr->isLocked()||lytblrcdPtr->isFrozen()||lytblrcdPtr->isOff())
	{
		acutPrintf(_T("\n%s层被锁定或关闭或冻结."),layername);    
		lytblrcdPtr->close();
		return ;
	}
	else 
	{
		lytblrcdPtr->close();  						
	}
	resbuf layerRb;
	layerRb.restype = 8;
	layerRb.resval.rstring = layername;
	layerRb.rbnext = NULL;
	flag = acedSSGet(_T("X"),NULL,NULL,&layerRb,ss);
	if(flag == RTNORM) 
	{
		acedSSLength(ss,&sslen);
		acutPrintf(_T("\n%s层%ld个实体被选中"),layername,sslen);
	}
	//else
	//{
	//	acdbNameClear(ss);
	//}  //最好由传入ss的函数来设置ss为nil;
}


//获取全部实体；
void CSelectEnts::getSSOfAllDrawing(ads_name ss)
{
	long sslen;
	ads_name ssUnit;
	long cal,ssDelNum = 0;
	long hasLocked = 0;
	acedSSGet(_T("A"),NULL,NULL,NULL,ss);  //选择全部图形。
	acedSSLength(ss,&sslen);
	if(sslen == 0)
	{
		acutPrintf(_T("\n没有选择任何实体!"));
	}
	else   //过滤实体集;
	{
		filterFrozenLockedOff(ss);
	}
}


//过滤掉被冻结的，被锁定的图形实体;
void
CSelectEnts::filterFrozenLockedOff(ads_name ss)
{
	AcDbLayerTable *lytblptr;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytblptr, AcDb::kForRead);
	AcDbLayerTableRecord *lytblrcdptr;

	long sslen,cal,ssDelNum;
	long hasLockLayer =0;
	ads_name ent;
	acedSSLength(ss,&sslen);
	for(ssDelNum = cal = 0;cal < sslen;cal++,ssDelNum++)
	{						
		AcDbEntity *EntObj;		 	  acedSSName(ss,ssDelNum,ent);
		AcDbObjectId objId;
		acdbGetObjectId(objId,ent);
		acdbOpenAcDbEntity(EntObj,objId,AcDb::kForRead);
		lytblptr->getAt(EntObj->layer(),lytblrcdptr,AcDb::kForRead);
		if(lytblrcdptr->isLocked()||lytblrcdptr->isOff()||lytblrcdptr->isFrozen())
		{							
			acedSSDel(ent,ss);													
			hasLockLayer++;	
			ssDelNum--;
		}
		EntObj->close();
		lytblrcdptr->close();
	}

	lytblptr->close();											
	if(hasLockLayer != 0)  
		acutPrintf(_T("\n有%ld个物体被锁定，无法选中它们"),hasLockLayer);
	if(ss)
	{
		acedSSLength(ss,&sslen);
		acutPrintf(_T("\n有%ld个物体被选中.\n"),sslen);
		//return true;
	}
}




//获取所有层的层名；
void
CSelectEnts::getAllLayerName()
{
	AcDbLayerTableRecord *lyrTblRcd;// = new AcDbLayerTableRecord;

	AcDbLayerTable *lyrTblPtr;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lyrTblPtr, AcDb::kForRead);
	AcDbLayerTableIterator *lyrTblIterator;

	lyrTblPtr->newIterator(lyrTblIterator);
	lyrTblPtr->close();

	for(lyrTblIterator->start();!lyrTblIterator->done(); lyrTblIterator->step())
	{ 
		lyrTblIterator->getRecord(lyrTblRcd,AcDb::kForRead);
		ACHAR* lyrname = new ACHAR[128];
		lyrTblRcd->getName(lyrname);
		m_allLayerName.push_back(lyrname);
		lyrTblRcd->close();
	}

	//delete lyrTblRcd;
	delete lyrTblIterator;	
}



//用户选完实体集后，因程序需要，将该实体集中某层的图形挑出来，形成另外一个实体集；
//m_ssByLayer;每执行一次此函数，遍历一个层名；
//按层名挑选；
bool
CSelectEnts::getSSAltLayer(ads_name ssAll,ads_name ss_byLayer)
{
	//acedSSFree(m_ssByLayer);//清除集合先；

	ACHAR* lyrName;
	int layerDepth = m_allLayerName.size();
	if(m_layerNumber < 0 || m_layerNumber > layerDepth)
	{
		return false;
	}
	else
	{
		lyrName = m_allLayerName[m_layerNumber];
	}


	AcDbEntity* pEnt;
	AcDbObjectId id;
	ads_name ssUnit;  //单个实体变量；

	long len;
	acedSSLength(ssAll,&len);
	for(long i = 0; i < len; i++)
	{
		acedSSName(ssAll,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenObject(pEnt,id,AcDb::kForRead);
		if(wcscmp(pEnt->layer(),lyrName) == 0)
		{
			acedSSAdd(ssUnit,ss_byLayer,ss_byLayer);     //此处缺调用返回错误检查；
		}
	}
	return true;
}
//=============用户选择实体类========================================end=======






