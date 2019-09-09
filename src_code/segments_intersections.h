/*************************************************************************
Copyright (C), 2017-12-1, shiyuan precision maskmaking. Co., Ltd.
FileName: pointMap.h
Author: 
Version:
Date: //zhg 1.0
Description: // 1.��ѡ�е�ʵ�弯�Ͻ������������ѯ;
             // 2.ʹ��map�ṹ; �Ե������x��yΪ��ѯkeyֵ; x��mapǶ��y��map��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
1. ....
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
		// �߼��޸����ݼ���
eg�� Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20190822_segments_intersections
#define  ZHOUHUAGANG_20190822_segments_intersections



#include <vector>
#include <map>   
#include <algorithm>
//#include <>


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

#include "..\StdAfx.h"
#include "TCHAR.h"

#include <vector>
#include <list>
#include <map>


//����3���ṹ��
//1.�洢�����߶εĶ˵�list�������·��ֵĽ���;
//2.�洢ɨ�����ϵĵ�set��������������
//3.�洢���ֵĽ���;

//��������㣺
//1.�˵��غϵ��߶���ô���;
//2.Բ����ô��ʾ���߶Σ�: ������ͨ�����ĵĴ��ߺ�ˮƽ�߻��ֳ��Ķ�; ÿ�λ��ǵ�����;
//3.�������¶˵㣻

//��������ܣ���N���߶εĽ��㼯;
//����ɨ�����ཻ�㷨;
//


//ָʾ���η�λ��
//������ͨ�����ĵĴ��ߺ�ˮƽ�߻��ֳ��Ķ�:���ϣ����£����ϣ�����;
enum EArcLabel
{
	LEFT_TOP,
	LEFT_BOTTOM,
	RIGHT_TOP,
	RIGHT_BOTTOM,
	NONE_NONE
};


/*
������CSegement
��;����ʾһ���߶Σ�������һ��ֱ�ߣ�Ҳ������һ����;   
һ����һ�߷ֽ�ɶ��ֱ�߶κͻ���
���λ��ֳ��ĶΣ�������ͨ�����ĵĴ��ߺ�ˮƽ�߻��ֳ��Ķ�; ÿ�λ��ǵ�����
*/
class CSegement
{
	AcDbPolyline::segType m_segmentType;
	AcGeLineSeg2d m_lineSeg;
	AcGeCircArc2d m_arcSegL1;
	//if it is arc segment
	EArcLabel m_arcLabel;
	AcGeCircArc2d m_arcSegL2;
	AcDbPolyline* m_polyline;
};


#endif  //ZHOUHUAGANG_20190822_segments_intersections