/*************************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: 
Version:
Date: // hugo��1.0
Description: // ��ͼ����arc��line��polyline��ȡ: 
Others: // �������ݵ�˵��
        1.0 ��ģ�����ϣ�ֻ����Ϊ�ο����㷨��������Ҳ������. 2017-9-7 �ܻ���;
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
eg�� Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20160612_SELECTSS_DOT_H
#define  ZHOUHUAGANG_20160612_SELECTSS_DOT_H


#include "..\StdAfx.h"
#include <vector>
#include <stack>    //��ջ��װ��·��(��)��
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
//=======����Ϊʵ��ѡ��\������=================================================================================
//�û�ѡ��ʵ��ֻ������ѡ�ALL/one layer/select��
//ѡ��ʵ����࣬�ṩ����ѡ��ʵ��ķ��������磬���㣬���û�ѡ��ȫ��ͼ�㣻
//����ߺͶ��㣬��ͼ��
class  CSelectEnts
{
public:
	CSelectEnts();
	~CSelectEnts();   

	void usrSelect(ads_name ss);  //�û�����ѡ��ALL,ONE LAYER ,SELECT��
	//���û�ѡ���ʵ�弯�����ϣ������ȡʵ�弯m_selByLayer��ÿ����һ�Σ��Զ������ȡʵ�壬ֱ���������в㣻
	//����false�������в�������ˣ�
	bool getSSAltLayer(ads_name ssAll,ads_name ss_byLayer);  

private:  //˽�к���
	void getSSOfOneLayer(ACHAR *layername,ads_name ss);   //��ȡĳ���ʵ�弯��
	void getSSOfUserSelect(ads_name ss);   //�û��������CAD����ѡ��
	void getSSOfAllDrawing(ads_name ss);   //��ȡ����ʵ�弯��
	void filterFrozenLockedOff(ads_name ss);     //���˵������ͼ���ʵ�壻//���˵�����ʾ��ͼ���ʵ�壻  //���˵�������ͼ���ʵ�壻
	void filterByLayer(ACHAR* layername);    //��ѡ�񼯹��ˣ����ϲ��������£�


	void getAllLayerName();  //��ȡ���в�Ĳ�����

private:
	//ads_name m_sel;  //�û�ѡ���ʵ�壨�����ģ�����ģ��ر���ʾ�Ķ����˵�����  
	//long m_sslen;    //�û�ѡ���ʵ�峤�ȣ�

	ACHAR* m_charUserInput;   //�û�������ַ�A,S��O��

	//������¼���в�����
	vector<ACHAR*> m_allLayerName;  //���в�Ĳ�����
	int m_layerNumber;  //�������в�����
	//ads_name m_ssByLayer;       //���û�ѡ���ʵ�弯m_sel�����ϣ�����ѡ�񣨹��ˣ�ʵ�弯��
	ACHAR* m_layerName;       //m_selByLayer���ڲ������
};


#endif  //ZHOUHUAGANG_20160612_SELECTSS_DOT_H

//=======����Ϊʵ��ѡ����=================================================================================
