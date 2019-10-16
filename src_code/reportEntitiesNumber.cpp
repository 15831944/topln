/************************************************************
Copyright (C), 2016-6-12, XuMiJieZi procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: Version :
Date:
Description:  1��line,arc������ȡ��

Version: // �汾��Ϣ;
Function List: // ��Ҫ�������书��
1. 
2.
3.
History: // ��ʷ�޸ļ�¼
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/


//#include "StdAfx.h"
#include  "reportEntitiesNumber.h"
#include "TCHAR.h"

//SData��ʼ����
SData::SData()
{
	isPolylineClosed = false;
	m_number = 0;
	m_numOfNotClosed = 0;
}




//���캯��
CEntitiesReport::CEntitiesReport()
{
}


//��������;
CEntitiesReport::~CEntitiesReport()
{
}


//��ʵ�弯���з���������������
void
CEntitiesReport::analysisEntity(ads_name ss)
{	
	wstring wstrInfo;  //�洢ʵ�����ͼ�������Ϣ��	

	long nlength = 0;
	acedSSLength(ss,&nlength); 
	if(nlength == 0)
	{
		acutPrintf(_T("���棺�ļ���û��ͼ��!\n"));
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

		m_sdt.m_number = 1;       //�����������
		m_sdt.m_numOfNotClosed = 1;   //�����������
		
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
		pEnt->close(); //�򿪺�ע��رգ�


		//SData����׼������֮��ͼ���vector��
		m_typeAndNumber.findAndUpdate(&m_sdt);
	}

	//m_typeAndNumber.printData();
	wstringstream wss;
	wss << _T("���ļ��ܹ�������");
	wss << nlength;
	wss << _T("��ͼ��;����\n");

	wstrInfo.clear();
	wstrInfo += wss.str();
	wstrInfo += m_typeAndNumber.printInfo();

	acutPrintf(wstrInfo.c_str());
	//m_typeAndNumber.printInfo();
}


//���캯��
CDataArray::CDataArray()
{
}


//��������
CDataArray::~CDataArray()
{
}



//�����Ƿ���ڴ�ʵ�����ͣ�������ڣ�������һ�������������Ŀ��
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
					//��Ӽ��������Ƴ�������
					itr->m_number++;
					return true;  //����true����ʾ���ڣ�����Ӽ�����
				}	
				else
				{
					itr->m_numOfNotClosed++;
					return TRUE;
				}			
			}
			else  //���Ƕ����ߣ�
			{
				itr->m_number++;
				return TRUE;
			}
		}
	}

	//��ѭ�������ˣ�˵��û���ҵ��ִ�ʵ�����ͣ�����Ҫ������ͣ�
	//insert;
	push_back(*psd);
	return false;
}


//��ʵ�����ͱ�������
void 
CDataArray::push_back(SData sdat)
{
	m_nameNumber.push_back(sdat);
}



//��ӡʵ�����ͼ����ݣ�
void
CDataArray::printData()
{
	acutPrintf(_T("ͼ��ʵ�����ͼ�������������:\n"));
	vector<SData>::iterator itr = m_nameNumber.begin();
	for(; itr != m_nameNumber.end(); itr++)
	{
		//��ӡ����������������Ƕ����ߣ���Ҫ�㱨�Ƿ�պϣ�
		if(itr->m_className == AcDbPolyline::desc()->name())
		{
			acutPrintf(_T("%s�պ�:����%d����\n"),itr->m_className.c_str(),itr->m_number);
			acutPrintf(_T("%s���պ�:����%d����\n"),itr->m_className.c_str(),itr->m_numOfNotClosed);   								
		}
		else		//�Ƕ����ߣ�
		{
			acutPrintf(_T("%s:����%d����\n"),itr->m_className.c_str(),itr->m_number);    
		}	
	}
}



//��ʵ�����ͼ�������Ϣ����wstring���أ�
//���ﲻ�ô�ӡ��
wstring
CDataArray::printInfo()
{
	std::wstringstream ss;
	wstring wsInfo;
	wsInfo.clear();
	wsInfo +=_T("ͼ��ʵ�����ͼ�������������:\n");  
	
	vector<SData>::iterator itr = m_nameNumber.begin();   
	for(; itr != m_nameNumber.end(); itr++)
	{
		//��ӡ����������������Ƕ����ߣ���Ҫ�㱨�Ƿ�պϣ�
		if(itr->m_className == AcDbPolyline::desc()->name())     
		{
			ss<< itr->m_className.c_str();
			ss << (_T("�պ�:����:"));
			ss<<itr->m_number;
			ss << _T("����\n");
			//wsInfo += ss.str();
			

			ss<< itr->m_className.c_str();
			ss << (_T("���պ�:����:"));			
			ss << itr->m_numOfNotClosed;
			ss << _T("����\n");
			//wsInfo += ss.str();									
		}
		else		//�Ƕ����ߣ�
		{
			ss<< itr->m_className.c_str();
			ss << (_T("����:"));			
			ss<<itr->m_number;
			ss << _T("����\n");
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
		acutPrintf(_T("ͼ��û��ʵ��.\n"));
		return;
	}
	//acedSSGet(_T("A"),NULL,NULL,NULL,ss);

	CTimeElapse objTime;  //ͳ������ʱ�䣻

	CEntitiesReport objReport;
	objReport.analysisEntity(ss);

	acedSSFree(ss);
}

//����¼
//1-��ӡ������wstring��¼�������ɣ�Ȼ��һ�����������Ļ��
//2-Ӣ�ı�ʾʵ�����Ͳ����ʣ��������ġ�
//3-ѡ��ʵ�弯����Ҫ��д���ܲ������࣬�û��ӿ������⣻
//4-�������ʱ��ͳ�ƣ�