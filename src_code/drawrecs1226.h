/*************************************************************************
Copyright (C), 2016-6-12,XuMiJieZi.
FileName: drawrecs1226.cpp
Author: 
Version:
Date: // hugo��1.0
Description: 
ĳ���ͻ�����wintopo�������bmpͼƬת����ʸ��ͼ��cad֮dwg��ʽ��
dwgͼ���ﶼ��С�����Σ��ص���һ��û�������ʵ�������������
���������⣻�����ǣ�������С�������ػ�һ��;
;
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
eg�� Date:	Author: 	Modification:
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
	vector<AcGePoint3d> m_allPoints; //���еĵ�;
	CPointMap m_ptMap; 
	vector<AcDbPolyline*> m_allPlines; //���ж�����;
public:
	void doit();  //executing;
private:
	void collectPoints(ads_name ss);
	void collectPointsOfLine(IN const AcDbEntity* pEnt);
	void collectPointsOfArc(IN const AcDbEntity* pEnt);
	void collectPointsOfPolyline(IN const AcDbEntity* pEnt);
	bool isPointRight(IN const AcGePoint3d pt3d); //���Դ˶����Ƿ���Ի�������;
	void drawRects(IN const AcGePoint3d pt3d);
	void drawAllRects();
	void join2db();
};







#endif  //ZHOUHUAGANG_DRAWRECS_DOT_H