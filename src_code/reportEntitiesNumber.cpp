/************************************************************
Copyright (C), 2016-6-12, XuMiJieZi procision maskmaking. Co., Ltd.
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


//#include "StdAfx.h"
#include  "reportEntitiesNumber.h"
#include "TCHAR.h"

//SData初始化；
SData::SData()
{
	isPolylineClosed = false;
	m_number = 0;
	m_numOfNotClosed = 0;
}




//构造函数
CEntitiesReport::CEntitiesReport()
{
}


//析构函数;
CEntitiesReport::~CEntitiesReport()
{
}


//对实体集进行分析；报告数量；
void
CEntitiesReport::analysisEntity(ads_name ss)
{	
	wstring wstrInfo;  //存储实体类型及数量信息；	

	long nlength = 0;
	acedSSLength(ss,&nlength); 
	if(nlength == 0)
	{
		acutPrintf(_T("报告：文件中没有图形!\n"));
		return;
	}

	AcDbEntity* pEnt;
	ads_name ssUnit;
	for(int i = 0 ; i < nlength; i++)
	{
		AcDbObjectId objId;
		acedSSName(ss,i,ssUnit);
		acdbGetObjectId(objId,ssUnit);
		acdbOpenAcDbEntity(pEnt,objId,AcDb::kForRead);
		wstring strTypeName;
		strTypeName = pEnt->isA()->name();
		m_sdt.m_className = strTypeName;

		m_sdt.m_number = 1;       //下面需调整；
		m_sdt.m_numOfNotClosed = 1;   //下面需调整；
		
		if(strTypeName == AcDbPolyline::desc()->name())
		{
			if(((AcDbPolyline*)pEnt)->isClosed())
			{
				m_sdt.isPolylineClosed = true;
				m_sdt.m_number = 1;
				m_sdt.m_numOfNotClosed = 0;
			}
			else
			{
				m_sdt.isPolylineClosed = false;
				m_sdt.m_number = 0;
				m_sdt.m_numOfNotClosed = 1;
			}
		}
		pEnt->close(); //打开后注意关闭；


		//SData数据准备好了之后就加入vector；
		m_typeAndNumber.findAndUpdate(&m_sdt);
	}

	//m_typeAndNumber.printData();
	wstringstream wss;
	wss << _T("此文件总共包含：");
	wss << nlength;
	wss << _T("个图形;其中\n");

	wstrInfo.clear();
	wstrInfo += wss.str();
	wstrInfo += m_typeAndNumber.printInfo();

	acutPrintf(wstrInfo.c_str());
	//m_typeAndNumber.printInfo();
}


//构造函数
CDataArray::CDataArray()
{
}


//析构函数
CDataArray::~CDataArray()
{
}



//查找是否存在此实体类型，如果存在，计数加一；否则添加新项目；
//
bool
CDataArray::findAndUpdate(SData* psd)  
{
	vector<SData>::iterator itr = m_nameNumber.begin();    
	for(; itr != m_nameNumber.end(); itr++)   
	{  
		if(psd->m_className == itr->m_className)
		{
			if(psd->m_className == AcDbPolyline::desc()->name())   
			{
				if(psd->isPolylineClosed == true)
				{
					//添加计数，并推出函数；
					itr->m_number++;
					return true;  //返回true，表示存在，并添加计数；
				}	
				else
				{
					itr->m_numOfNotClosed++;
					return TRUE;
				}			
			}
			else  //不是多义线；
			{
				itr->m_number++;
				return TRUE;
			}
		}
	}

	//从循环出来了，说明没有找到现存实体类型；则，需要添加类型；
	//insert;
	push_back(*psd);
	return false;
}


//向实体类型表添加新项；
void 
CDataArray::push_back(SData sdat)
{
	m_nameNumber.push_back(sdat);
}



//打印实体类型及数据；
void
CDataArray::printData()
{
	acutPrintf(_T("图形实体类型及数量报告如下:\n"));
	vector<SData>::iterator itr = m_nameNumber.begin();
	for(; itr != m_nameNumber.end(); itr++)
	{
		//打印类名、数量、如果是多义线，还要汇报是否闭合；
		if(itr->m_className == AcDbPolyline::desc()->name())
		{
			acutPrintf(_T("%s闭合:数量%d个；\n"),itr->m_className.c_str(),itr->m_number);
			acutPrintf(_T("%s不闭合:数量%d个；\n"),itr->m_className.c_str(),itr->m_numOfNotClosed);   								
		}
		else		//非多义线；
		{
			acutPrintf(_T("%s:数量%d个；\n"),itr->m_className.c_str(),itr->m_number);    
		}	
	}
}



//将实体类型及数量信息做成wstring返回；
//这里不用打印；
wstring
CDataArray::printInfo()
{
	std::wstringstream ss;
	wstring wsInfo;
	wsInfo.clear();
	wsInfo +=_T("图形实体类型及数量报告如下:\n");  
	
	vector<SData>::iterator itr = m_nameNumber.begin();   
	for(; itr != m_nameNumber.end(); itr++)
	{
		//打印类名、数量、如果是多义线，还要汇报是否闭合；
		if(itr->m_className == AcDbPolyline::desc()->name())     
		{
			ss<< itr->m_className.c_str();
			ss << (_T("闭合:数量:"));
			ss<<itr->m_number;
			ss << _T("个；\n");
			//wsInfo += ss.str();
			

			ss<< itr->m_className.c_str();
			ss << (_T("不闭合:数量:"));			
			ss << itr->m_numOfNotClosed;
			ss << _T("个；\n");
			//wsInfo += ss.str();									
		}
		else		//非多义线；
		{
			ss<< itr->m_className.c_str();
			ss << (_T("数量:"));			
			ss<<itr->m_number;
			ss << _T("个；\n");
			//wsInfo += ss.str();
		}	
	}
	wsInfo += ss.str();

	//acutPrintf(wsInfo.c_str());
	return wsInfo;
}



void 
reportAllEnt()
{
	ads_name ss;
	CSelectEnts objSel;
	objSel.usrSelect(ss);
	long sslen;
	acedSSLength(ss,&sslen);
	if(sslen == 0)
	{
		acutPrintf(_T("图中没有实体.\n"));
		return;
	}
	//acedSSGet(_T("A"),NULL,NULL,NULL,ss);

	CTimeElapse objTime;  //统计运行时间；

	CEntitiesReport objReport;
	objReport.analysisEntity(ss);

	acedSSFree(ss);
}

//备忘录
//1-打印内容用wstring记录下来即可；然后一次性输出到屏幕；
//2-英文表示实体类型不合适，换成中文。
//3-选择实体集的类要重写，很不方便简洁，用户接口有问题；
//4-添加运行时间统计；