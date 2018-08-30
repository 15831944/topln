/************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: selectss.cpp
Author: Version :
Date:
Description:  1��line,arc������ȡ;

Version: // �汾��Ϣ;
Function List: // ��Ҫ�������书��
1. 
2.
3.
History: // ��ʷ�޸ļ�¼
<author> <time> <version > <desc>
David 96/10/12 1.0 build this moudle
***********************************************************/
#include "..\StdAfx.h"
#include  "to_pl.h"
#include "TCHAR.h"
#include "selectss.h"

//=============�û�ѡ��ʵ����========================================start=======

//�û�ѡ��ʵ�壻��װ�ࣻ
CSelectEnts::CSelectEnts()
{
	//��ȡ�û�ѡ��;
	m_charUserInput = new ACHAR[133];

	int flag;
	flag = acedGetString(0,_T("Select/One layer/<All layer>:"),m_charUserInput);

	//��ȡȫ��ͼ�������
	getAllLayerName();
}


//��ʾ�û�ѡ��drawing�е�ͼ�Σ�
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
	m_layerNumber = -1;  //��ʼ��Ϊ-1��
}


//����������
CSelectEnts::~CSelectEnts()
{
	//�ͷŲ���������ڴ棻
	ACHAR* pstr;
	int sizeAchar = m_allLayerName.size();
	for(int i  = 0 ; i < sizeAchar; i++)
	{
		pstr = m_allLayerName[i];
		delete[] pstr;
	}

	//�ͷ�ʵ�弯��
}



//�û��Լ�ѡ��ʵ�壻
void
CSelectEnts::getSSOfUserSelect(ads_name ss)
{
	int flag;
	flag = acedSSGet(NULL,NULL,NULL,NULL,ss);
	if(flag == RTCAN) 
	{
		return;
	}

	filterFrozenLockedOff(ss); //���ˣ�

	long sslen;
	if(ss)
	{
		acedSSLength(ss,&sslen);
		acutPrintf(_T("\n��%ld�����屻ѡ��"),sslen);
		//return true;
	}
}


//���ݲ�������ȡ�ò�ʵ�弯��
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
		acutPrintf(_T("\n%s�㱻������رջ򶳽�."),layername);    
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
		acutPrintf(_T("\n%s��%ld��ʵ�屻ѡ��"),layername,sslen);
	}
	//else
	//{
	//	acdbNameClear(ss);
	//}  //����ɴ���ss�ĺ���������ssΪnil;
}


//��ȡȫ��ʵ�壻
void CSelectEnts::getSSOfAllDrawing(ads_name ss)
{
	long sslen;
	ads_name ssUnit;
	long cal,ssDelNum = 0;
	long hasLocked = 0;
	acedSSGet(_T("A"),NULL,NULL,NULL,ss);  //ѡ��ȫ��ͼ�Ρ�
	acedSSLength(ss,&sslen);
	if(sslen == 0)
	{
		acutPrintf(_T("\nû��ѡ���κ�ʵ��!"));
	}
	else   //����ʵ�弯;
	{
		filterFrozenLockedOff(ss);
	}
}


//���˵�������ģ���������ͼ��ʵ��;
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
		acutPrintf(_T("\n��%ld�����屻�������޷�ѡ������"),hasLockLayer);
	if(ss)
	{
		acedSSLength(ss,&sslen);
		acutPrintf(_T("\n��%ld�����屻ѡ��.\n"),sslen);
		//return true;
	}
}




//��ȡ���в�Ĳ�����
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



//�û�ѡ��ʵ�弯���������Ҫ������ʵ�弯��ĳ���ͼ�����������γ�����һ��ʵ�弯��
//m_ssByLayer;ÿִ��һ�δ˺���������һ��������
//��������ѡ��
bool
CSelectEnts::getSSAltLayer(ads_name ssAll,ads_name ss_byLayer)
{
	//acedSSFree(m_ssByLayer);//��������ȣ�

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
	ads_name ssUnit;  //����ʵ�������

	long len;
	acedSSLength(ssAll,&len);
	for(long i = 0; i < len; i++)
	{
		acedSSName(ssAll,i,ssUnit);
		acdbGetObjectId(id,ssUnit);
		acdbOpenObject(pEnt,id,AcDb::kForRead);
		if(wcscmp(pEnt->layer(),lyrName) == 0)
		{
			acedSSAdd(ssUnit,ss_byLayer,ss_byLayer);     //�˴�ȱ���÷��ش����飻
		}
	}
	return true;
}
//=============�û�ѡ��ʵ����========================================end=======






