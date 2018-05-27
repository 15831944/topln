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
#ifndef  ZHOUHUAGANG_20160612_TO_PL_DOT_H
#define  ZHOUHUAGANG_20160612_TO_PL_DOT_H



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

#include  "opt.h"
#include "elapsetime.h"
#include "basefunc.h"
#include "pointMap.h"

#define  DEBUG_TO_PL_PRINT

//#pragma comment("rxapi.lib")
void to_polylines(void);

using namespace std;

class CEdge;
class CArcLinkArray;

//˼·��
//1��ʹ�õ�����洢�����߸��Σ�line��arc��pline��;
//2�����ܱպϵ�ͳͳ���������;
//3�����ڸ��ӵ���·�������Ժ���;
//3.1���磺�����·��ɵ����� �����˵����ܽ���tolerance֮�ڣ����ǲ��غϣ�;
//3.2���磺����֮���н��棨�ǲ���Ӧ�ô�Ϻ���ȡ�������������������������ͬ�û������ı�Ҫ;


//ָʾʵ������;
//ֻ���line��arc��polyline��not closed������topl;
//���ö�ٲ����ˣ�objectarx�ж��壻
//enum EntType
//{
//	LINETYPE,
//	ARCTYPE,
//	POLYLINETYPE
//};


//��Ա߽ṹ��ʵ�������Ƿ��v1 �� v2����v1��v2�Ǳ߽ṹ��CVertex��
//���ڿ���ʶ����Ҫ�𣿣���
enum ENTDirection
{
	REVERSE,
	SYNOTROPY
};



//=========================================
//��ʾһ���߱�;
//����ͼ�Ĺ����У��ռ�ͼ�еĸ���·����·���ɱ���ɣ�����·���������polyline��closed��not closed��
//1.���룺��ͼ�б���������·��;
//2.·�������˼·��
//2.1 �������·�����ص�������ȡ�ص�·�����м����ص���ȡ���㣻ÿ������һ��·��;
//2.2 Ȼ���ÿ����������ȡ�ص��ߣ�ÿ2����ȡһ�Σ���һ��·����
//2.3 ����ʣ�µģ��������ı߼��������ıߣ��ֱ�����·��;
//3.0 ÿ���߶�����գ���Ҫ����·��;  ·���ͱ�����;
//4.0����2.1��2.2��2.3˳��Ҳ���Ե���Ϊ��2.2��2.1��2.3;
//=========================================
class CArcLink
{
private:
	vector<int>  m_vectroIndex;  //��ͼ�������һ��·��������Ķ����ϵ�У���������ò��ϡ��Ժ�ɾ��;
	vector<CEdge*> m_edgesLink;   //��ͼ����ȡ��һ��·����
	//vector<CEdge*> m_edgesPathToPolyline();  // ��m_edgesLink���������ֱ���������ɶ����ߡ�
	//vector<vector<CEdge*>> m_pathToPolyline;    // ��m_edgesLink���������ֱ���������ɶ����ߡ�
	bool isClosed;  // ָʾ�Ƿ��ǻ�·;
	int m_numOfLoops;  //����path����С��ȣ���ʵҲ���Ǽ��pedge���ص��ı������Ǽ��㣻
public:
	CArcLink();  
	~CArcLink();   
	
public:
	bool isPathClosed(); //�жϱ�·���Ƿ�պ�·����
	void setClosed(IN bool isclosed); //·�������䶯��������Ӧ�պϲ���.  
	void push_back(IN CEdge* pedge);
	void push_front(IN CEdge* pedge);  //ͷ������; 
	CEdge* pop_edge();
	CEdge* back(); //ȡβ��Ԫ��; 
	int pop_index();
	void push_back(IN int index); 
	int sizeOfEdges();
	void clear();  //���;
	
	//�����·����·����������·�����ܴ���β�ͣ���Ҫ�������ȥ����ʣ�µ���������·��
	//β������DFS��ȱ�����ɵģ�
	void correctLinks(CArcLinkArray* pLinkArr); //��closed�ı�·������������,ȥ���ǻ����֣�//û���ϣ��㷨����.
	void numOfLoops();    //����·���м����ص�������·���ص���������·���ص�������
	void extractLoops(vector<vector<CEdge*>> & pEdgesLinks);  // ���ȶԱպϵ�arclink��ȡ�պ�Ȧ���м�Ȧ��ȡ��Ȧ��ʣ�µ����⺯������//û���ϣ��㷨����.
	void extractLoopsOfOverlappedEdges(OUT vector<vector<CEdge*>> & pEdgesLinks);// ��Σ���ȡ�ص����ϵĻ������������ص�����ɵĻ���
	void extractPathNotClosed(vector<vector<CEdge*>> & pEdgesLinks);  //�����ȡʣ�µ�·�������ǲ��պϵģ�
	void justExtractEdgesLink(OUT vector<vector<CEdge*>> & pEdgesLinks);

	//��ȡ·���󣬵�һ��·�������һ��·�������ܡ�Ҫ�ϲ����У�
	bool isEdgeLeftAfterLoopsExtacted(const CEdge* pEdge);  //�ж�һ��������ȡ�����Ƿ�Ϊ�ձ�;
	bool isFirstAndLastEdgeLinked();  //�жϴ�arclink��ͷ�ߺ�β���Ƿ���������ȡ�ص��ߺ�;
	bool getStartPointIndex(OUT int& iIndex); //��ȡ�������; ���û����㣨���Σ����򷵻�-1;
	bool getEndPointIndex(OUT int& iIndex);
	bool mergeFirstAndLastLink(vector<vector<CEdge*>> & pEdgesLinks); //���arclink�պϣ��ҵ�һ�κ����һ�������ڵģ���ϲ���һ���κ����һ�Σ���Ϊһ���Σ�
	

	//����polyline�� ���������һ����·polyline��Ҳ�����Ƕ���polyline��
	//bool to_polyline();

	//�����õĺ������������ɾ����
	void testPintOriginalPath(); //��ӡ��ͼ������ȱ�����õ���·����
};


//=================================
//�߱���;
//����·�����������ˣ��������� ��ͼ�����Ѿ�����;
//=================================
class CArcLinkArray
{
private:
	vector<CArcLink> m_edgeLinkArray;  //���·����
	//vector<CArcLink* > m_edgeLinkArrayToPolyline;  //��m_edgeLinkArray����������õ���ֱ���������ɶ����ߵģ�
	vector<vector<CEdge*>> m_edgeLinkArrayToPolyline ;   //��m_edgeLinkArray����������õ���ֱ���������ɶ����ߵģ��ò����ˣ����ϣ�
	vector<AcDbPolyline*> m_vecPolylines;  //�������ɵĶ����ߡ�	
public:
	CArcLinkArray();  //���캯��; 
	~CArcLinkArray();

public:	
	bool push_back(CArcLink edgesLink);  //����;
	void pop_back();  //����;

	// �����·CArcLink����·����������·�����ܴ���β�ͣ���Ҫ�������ȥ����ʣ�µ���������·��
	// β������DFS��ȱ�����ɵģ�	
	bool correctByRealEdges();    //����ʵ�ʱ�ʵ����������µ�·��path��	

	//���ɶ����ߣ�
	void toPolylines();   //��m_edgeLinkArrayToPolyline�ӹ������ɶ����ߡ� 
	AcDbObjectId join2database(AcDbEntity *pEnt);  //

	//�����õĺ������������ɾ����
	void testPrintOriginalPath(); //��ӡ��ͼ������ȱ�����õ���·���� 
	//�����������ɶ�һ�ߵ�m_edgeLinkArrayToPolyline���ݣ�
	void testm_edgeLinkArrayToPolyline();
};
//=======����Ϊ��·��=================================================================================




//=======����ΪCToPolyline��=================================================================================

//��vector<CEdge*>ת���ɶ������ࣻ
//vector<CEdge*>Ϊ�Ѿ�����õ�·��path��������Ҫʶ��CEdge֮��Ķ˵�Ӳ�˳��
class CToPolyline
{
private:
	vector<CEdge*>* m_edgeLink; 
	AcDbPolyline* pLine;
	vector<AcDb2dVertex*> m_vertexArr;  //����װ���ڴ�;
	int m_startPointIndex;  //temp var; ���ڼ�¼ȡ��������У�ĳ�ߵ����Ķ������;
	
	bool m_isclosed;  //ָʾ�������Ƿ�պ�;
	int m_nNumOfEdges; //m_edgeLink�м�����;

public:
	CToPolyline(vector<CEdge*>*& pEdgeLink,IN const AcGeTol equalPointTol);
	~CToPolyline();
	CToPolyline();
	void inputEdgelink(vector<CEdge*>* pEdgeLink);
	void to_polyline(IN AcDbPolyline*& pLine);  //���ɶ�����; 

private:
	//��·���ӹ���һϵ�ж���AcDb2dVertex��
	void to_vertex2d();
	void getStartVertexIndex();   //��ȡ��һ����������; 
	int getFirstVertexIndex();  //����������;
	int getLastVertexIndex();  //����յ㶥�����;
	void transEdgeToVetex(CEdge* pEdge);
	
	bool isPathClosed();  // ���ж��㶼����֮���жϸö������Ƿ�պ�;
	// ��ĳ����CEdge�Ƕ����ߣ����պϵģ������轫�����߶�������¶����ߡ�
	void insertPolyline(AcDbPolyline* pline,bool isReverse);   // isReverse��ʾ�Ƿ���Ҫ��pline�����������¶����ߣ�
	void insertLine(IN const CEdge* pEdge);
	void insertArc(IN const CEdge* pEdge);
	void insertPolyline(IN const CEdge* pEdge);
	//ȡ����bugle���ж��Ƿ�ӻ�����㻹���ص�������bugle������һ��;
	double getArcBugle(AcDbArc* pArc, bool isFromStartpoint); 
	bool getLastVertex(); //ȡ���һ�����㣬����vertex;

	//��ȡ��һ��edge����ĵ�һ������;  //������;
	void getFirstPosition();  //������;
	void getFirstVertex(CEdge* pedgeFirst,CEdge* pedgeSecond); //������;
	void getNextVertex(CEdge* pedge);  //������;
	void getLastVertex(CEdge* pedge);  //������;
	void inputEquPntTol(IN const AcGeTol equalPointTol);   //������;
	//����������ж������ֵ; //������;
	AcGeTol m_equalPoint;  //������;
	bool isVetexListClosed();  //������;

private:
	void printVertex(IN AcDb2dVertex*& pt);  //���ԣ���ӡ������Ϣ��   
	void printEdgeLink();   //��ӡ�߱���Ϣ;   
};
//=======����ΪCToPolyline��=================================================================================



//=======����Ϊͼ��=================================================================================

//����;��CAD�Ķ�������ΪDATA�����ö����ڽӱ�
class CVertex
{
public:
	AcGePoint3d pt;    //autocad�ĵ㣻
	int m_degree;  //��;
public:
	CEdge* adj;   //���ӱ�;

public:
	CVertex(const AcGePoint3d& ptinput);	// ���캯����
	CVertex();
	~CVertex();
	bool operator == (CVertex & v1);
	void getPoint(AcGePoint3d& ptretn);  // ���ض������ꣻ
};



//���ࣻ�߿�����line��arc��polyline֮һ��closed polyline���������㣻
//�������ӱ��ʾ��(����ʮ��������
class  CEdge
{
public:
	CEdge(AcDbEntity* pent);  //һ��ʵ��ͼԪ����һ����;
	CEdge();
	~CEdge();

public:
	int index1;    //����1��ͼ�е���ţ�
	int index2;     //����2��ͼ�е���ţ�
	CEdge* path1;  //��һ���������һ���ڽӱߣ� 
	CEdge* path2;   //�ڶ����������һ���ڽӱߣ�  

	AcGePoint3d ptstart;  //�߶���㣻
	AcGePoint3d ptend;   //�߶��յ㣻
	CEdge* ptrSameEdges;  //ͬһ���߿����ж��ʵ��ͼԪ��line��arc��polyline��;
	AcDbEntity* ptrEnt;
	AcGe::EntityId enttype;   //����ʵ����ʲô�ࣺline��or arc��polyline������ 
	//AcDbEntity* pEnt;   //ָ��һ��ʵ�壻ָ��Ƚ�ʡ�ռ䡣 

	int m_numSameEdges;   //�˱ߣ�ͼ�ṹ�����м���ʵ��ͼԪ�ߣ��ص���;
	int m_leftSameEdges;  //ͳ�ƴ����߱�����֮�󣬻�ʣ��������Ϊ0�����������ˣ������õ��������������ȡ��·; 

public:
	bool m_isJoinedIntoNewPolyline;  //��Ǵ�ʵ���Ƿ�������µĶ����ߣ������topl֮��); 
	CEdge* getNextPathPtr(IN const int iIndex);  //����������ţ���index1��index2�Աȣ���������path1����path2;
	bool setNextPathPtr(IN const int iIndex,IN CEdge* pEdge);  //����path1��path2��ֵ;���ı�pathָ��;
};



//����ʵ�弯��ȷ��������߼��������꣬����������ֵ���𣬹���;   
//��������ȵ����ֵ��
//
class CGetPointEqualAcGeTol
{
private:
	double m_tol;   //���ֵ;
public:
	CGetPointEqualAcGeTol();  
	~CGetPointEqualAcGeTol();  

public:
	AcGeTol getPointTolerance(IN ads_name ss,OUT int& nDotNum);  
};


//ͼ��,�洢����ʵ��;
//���ö�������;
//���㰴int���;
//�㷨�޸ģ�Ӧ���ڱ���������֮ǰ��ȡ���ظ��ߡ�������ᵼ�����������鲻��;(��Ȼ��Ҳ��һ��ѡ�����)
class  CGraphEnts    //���е�ͼ�Σ�line��arc��polyline�������������ͼ�ṹ;  
{
	//friend class CArcLink;
	//friend class CArcLinkArray;  //��Ԫ�࣬Ϊ�˷��ʸ����˽�г�Ա������ͼ�ṹ; 
private:
	int MaxNumVertexs;    //��󶥵���;
	int numVertexs;      //Ŀǰʵ�ʶ�����;  
	vector<CVertex* > m_vertexTable;  //�����; 
	vector<CEdge*> m_vctEdges;  //�߱��ռ��û�������ڴ棬��Ҫ�����ͷ����ڴ档 
	int numEdges;         //ͼ�ı���;
	ads_name m_sel;      //�����ۺ϶����ߵ�ʵ��;���˺�Ŵ��뱾��;    
	long m_ssLength;   // ʵ�弯����(ʵ���);
	CArcLink  m_stackEdges;  //��ȱ���ͼ�����У���ű����ı�ϵ�е�ջ;  
	CArcLinkArray m_allLoops;  //���еĿ��ܵ�·����װ���ڴ�; 

	vector<int> visited;    //��¼�ڵ��Ƿ񱻷��ʹ�;	

	CPointMap m_pointMap;  //�洢������;
	int m_nDotNum;  //������С��λ��������λ.  

private:
	vector<CEdge*> m_paths;  //��ʱ������ȱ����󣬴�carclinkarray��ȡ��·��������Ƿ���ڣ���ʵ��·������

public:
	CGraphEnts();
	~CGraphEnts();   //��������;  

public:
	void createGraph(); //��ͼ;
	//��ͼ���и��죬ÿ���ȴ���2�Ķ��㶼������ʹ��ÿ������Ȳ�����2;
	//��ȡ·�����պϼ����պϣ�:a-�ص�������ȡ��·��;b-Ȼ����ȡ����·��;  
	//��·�����ɶ�����;  
	void to_polyline(IN ads_name ssPick); 
	static bool isTwoPointsEqual(IN const AcGePoint3d& pt1,IN const AcGePoint3d& pt2,IN const int nDotNUm); 
	
private:
	int insertVertex(IN AcGePoint3d &pt);     //��ͼ�в���һ������;    
	void insertEdge(CEdge* pedge);    //��ͼ�в���һ����;   
	int delVertex(int v);  //ɾ��ͼ��һ������(����ر�ҲҪɾ��);     
	bool delEdge(CEdge* pedge);   //ɾ��ͼ��һ����;  

private:
	int GetFirstNeighbor(int v1,OUT CEdge*& pedge);  //ȡ����v1�ĵ�һ���ڽӶ��㣻
	int GetNextNeighbor(const int v1,const int v2,CEdge*& pedge);  //ȡ����v1�����ڽӶ���v2����ڽӶ���v3;
	int findVertexNode(const AcGePoint3d& pt);  //ͨ�������pt��ö���λ��;
	CEdge* findEdge(const AcGePoint3d& pt1,const AcGePoint3d& pt2); 
	CEdge* findEdge(IN const CEdge* pedge);  //pedge�ǻ�û����ͼ��������֪�䶥����ż����㣻
	int getDegreeOfVertex(int index);  //��ȡĳ������Ķȣ�������ߺͳ��ߵ������ͣ�����ͼû�г��ߺ����֮�֣�ֻ����رߣ���
	void testIfLoopONextVtxNode(const int index);  /*,CArcLink& objCArcLink);*/ //�������иö�����ڽӵ㣬ȷ���Ƿ��γɻ�����·����

	bool createEdge(CEdge*& pedge,ads_name ssunit);  //����ʵ��ssunit����һ����edge���󣻱�����ߺ�����insertEdge�����ã�
	void resetvisited();// ��������visited[];

	void  extractEdgeLinks(IN const int iIndex);  //ÿ������Ķȶ���<=2��ֱ����ȡÿ���߱���;
	//��ֱ���������ȴ���2�ı߱�Ҫ��֣�����¶��㣬��ֳɶ���߱�;
	void chopEdgeLinks();
	int insertSameVertex(IN const int vtxIndex);  //�����¶��㣬�˶���϶����ظ�����(��ֳ����Ķ���);
	void chopVertex(IN IN const int iIndex);
	//ȡĳ����iParent�ĳ�v1�����һ���㣬ǰ���Ǹö����Ϊ2;
	int getAnotherVertex(IN const int v1,IN const int iParent,IN CEdge*& pEdge); 
	bool isVertexIndexValid(IN const int iVertexIndex);   
	bool extractOverlappedEdges(); 	
	bool extractOverlappedEdge(IN CEdge* pEdge); //��ָ������ȡ�ص��߹��ɵĻ�·;  

	//��ȱ������õݹ鷽ʽ��������ջ�ṹ��ʹ��ѭ��������;   
	//index��ĳ��������ţ���ͼ����ͨ�ģ�ֻ����һ��;  
	void  DFS(int index);    //��ȱ�������ȡ�߱�; 
	void DFS2(int iIndex);  //��Ϊ���ж���ֻ��1-2�ȣ�����ֻ��ȡ�߱�����Ҫ����ȱ�����
	//���ԣ�
	void testDFS3(IN int iIndex);  //����ͼ������߱�; ����������;  
	void DFS3();  //����testDFS3�����ͼ����;���ԣ�
	
private:
	void prsDotNumAndTol();  //����������С��λ��������ȡ���ֵ;

	//���Ժ�������ӡ�ߵ���Ϣ��
	void static printEdgeInfo(CEdge* pEdge);
};


//��DFS�����У�stack������·����1.���л�··��������Ҫ��ȡ��
//��ȡ��·�󣬱�����stack�У�Ȼ�����ɱպ϶�һ�ߣ�
//2.�����ǻ�·��Ҳ�������ӳɷɱպ϶�һ�ߣ�

#endif  //ZHOUHUAGANG_20160612_TO_PL_DOT_H





/* �������̱���¼��topln��
2017-11-17 �������ⱸ��¼��
1-���������ж�������
1.1 equal���������ֵȡ�ã�
1.2 equal���ã���Ҫͳһʹ��һ�����ֵ��
1.3 ���ֵҪ����ͼ�δ�С��λ��;

2��ʵ�弯ѡ����Ҫ��������

3.  �����߹�ϵ��飺1.�ཻ��2.ȱ�ڣ���ΪԤ�ȴ���;
3.1 ��ʾ�û��޸�;

4.0
4.1���ֻ�������ص��ߵ�����£�����������
�������ߣ� ������Ϊ����ʶ��Ϊ2����·��

5.0
5.1 ���ɶ����ߵĲ����ã�

6.0
6.1 
*/