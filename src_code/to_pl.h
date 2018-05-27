/*************************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
FileName: to_pl.cpp
Author: 
Version:
Date: // hugo、1.0
Description: // 对图形中arc，line，polyline提取: 
Others: // 其它内容的说明
        1.0 此模块作废，只能作为参考；算法不完整，也不完善. 2017-9-7 周华刚;
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
1. ....
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改
		// 者及修改内容简述
eg： Date:	Author: 	Modification:
1. ...
2.
*************************************************************************/
#ifndef  ZHOUHUAGANG_20160612_TO_PL_DOT_H
#define  ZHOUHUAGANG_20160612_TO_PL_DOT_H



#include <vector>
#include <stack>    //用栈来装载路径(边)；
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

//思路：
//1、使用单链表存储轮廓线各段（line，arc，pline）;
//2、不能闭合的统统放入问题层;
//3、对于复杂的线路，留待以后解决;
//3.1比如：多个回路组成的网； 线条端点距离很近（tolerance之内，但是不重合）;
//3.2比如：线条之间有交叉（是不是应该打断后提取“环”？），特殊情况，存在同用户交互的必要;


//指示实体类型;
//只针对line，arc，polyline（not closed）进行topl;
//这个枚举不用了，objectarx有定义；
//enum EntType
//{
//	LINETYPE,
//	ARCTYPE,
//	POLYLINETYPE
//};


//针对边结构中实体走向是否从v1 到 v2反向，v1和v2是边结构的CVertex；
//便于快速识别？需要吗？？？
enum ENTDirection
{
	REVERSE,
	SYNOTROPY
};



//=========================================
//表示一个边表;
//遍历图的过程中，收集图中的各种路径；路径由边组成；合理路径最后做成polyline（closed获not closed）
//1.输入：从图中遍历而来的路径;
//2.路径整理关思路：
//2.1 如果整个路径有重叠，先提取重叠路径，有几层重叠提取几层；每层生成一个路径;
//2.2 然后对每个边链表提取重叠边，每2个提取一次，做一个路径；
//2.3 还有剩下的，对连续的边及不连续的边，分别做成路径;
//3.0 每条边都不落空，都要做成路径;  路径就边链表;
//4.0以上2.1，2.2，2.3顺序也可以调整为：2.2，2.1，2.3;
//=========================================
class CArcLink
{
private:
	vector<int>  m_vectroIndex;  //从图中提起的一条路径所代表的顶点的系列；这个可能用不上。稍后删除;
	vector<CEdge*> m_edgesLink;   //从图中提取的一条路径；
	//vector<CEdge*> m_edgesPathToPolyline();  // 从m_edgesLink整理而来，直接用于生成多义线。
	//vector<vector<CEdge*>> m_pathToPolyline;    // 从m_edgesLink整理而来，直接用于生成多义线。
	bool isClosed;  // 指示是否是环路;
	int m_numOfLoops;  //整条path的最小深度；其实也就是检查pedge中重叠的边最少是几层；
public:
	CArcLink();  
	~CArcLink();   
	
public:
	bool isPathClosed(); //判断本路径是否闭合路径。
	void setClosed(IN bool isclosed); //路径经过变动后，设置相应闭合参数.  
	void push_back(IN CEdge* pedge);
	void push_front(IN CEdge* pedge);  //头部插入; 
	CEdge* pop_edge();
	CEdge* back(); //取尾部元素; 
	int pop_index();
	void push_back(IN int index); 
	int sizeOfEdges();
	void clear();  //清空;
	
	//整理回路：回路不是完整回路，可能带有尾巴；需要将多余边去掉，剩下的是完整回路；
	//尾巴是由DFS深度遍历造成的；
	void correctLinks(CArcLinkArray* pLinkArr); //对closed的边路径：初步整理,去掉非环部分；//没用上；算法改了.
	void numOfLoops();    //检查此路径有几个重叠（就是路径重叠数量），路径重叠，懂吗？
	void extractLoops(vector<vector<CEdge*>> & pEdgesLinks);  // 首先对闭合的arclink提取闭合圈；有几圈提取几圈；剩下的另外函数处理；//没用上；算法改了.
	void extractLoopsOfOverlappedEdges(OUT vector<vector<CEdge*>> & pEdgesLinks);// 其次，提取重叠边上的环；即由两条重叠边组成的环；
	void extractPathNotClosed(vector<vector<CEdge*>> & pEdgesLinks);  //最后，提取剩下的路径，都是不闭合的；
	void justExtractEdgesLink(OUT vector<vector<CEdge*>> & pEdgesLinks);

	//提取路径后，第一条路径和最后一条路径“可能”要合并才行；
	bool isEdgeLeftAfterLoopsExtacted(const CEdge* pEdge);  //判断一条边在提取环后，是否为空边;
	bool isFirstAndLastEdgeLinked();  //判断此arclink的头边和尾边是否连续（提取重叠边后）;
	bool getStartPointIndex(OUT int& iIndex); //获取起点的序号; 如果没有起点（环形），则返回-1;
	bool getEndPointIndex(OUT int& iIndex);
	bool mergeFirstAndLastLink(vector<vector<CEdge*>> & pEdgesLinks); //如果arclink闭合，且第一段和最后一段是相邻的，则合并第一个段和最后一段，成为一个段；
	

	//生产polyline； 结果可能是一个回路polyline，也可能是多条polyline；
	//bool to_polyline();

	//测试用的函数；测试完可删除；
	void testPintOriginalPath(); //打印从图里面深度遍历后得到的路径；
};


//=================================
//边表集合;
//不对路径进行整理了，无需整理； 在图类中已经整理;
//=================================
class CArcLinkArray
{
private:
	vector<CArcLink> m_edgeLinkArray;  //多个路径；
	//vector<CArcLink* > m_edgeLinkArrayToPolyline;  //在m_edgeLinkArray基础上整理得到，直接用来生成多义线的；
	vector<vector<CEdge*>> m_edgeLinkArrayToPolyline ;   //在m_edgeLinkArray基础上整理得到，直接用来生成多义线的；用不上了，作废！
	vector<AcDbPolyline*> m_vecPolylines;  //最终生成的多义线。	
public:
	CArcLinkArray();  //构造函数; 
	~CArcLinkArray();

public:	
	bool push_back(CArcLink edgesLink);  //纳入;
	void pop_back();  //弹出;

	// 整理回路CArcLink：回路不是完整回路，可能带有尾巴；需要将多余边去掉，剩下的是完整回路；
	// 尾巴是由DFS深度遍历造成的；	
	bool correctByRealEdges();    //根据实际边实体来整理出新的路径path；	

	//生成多义线；
	void toPolylines();   //对m_edgeLinkArrayToPolyline加工，生成多义线。 
	AcDbObjectId join2database(AcDbEntity *pEnt);  //

	//测试用的函数；测试完可删除；
	void testPrintOriginalPath(); //打印从图里面深度遍历后得到的路径； 
	//测试用来生成多一线的m_edgeLinkArrayToPolyline内容；
	void testm_edgeLinkArrayToPolyline();
};
//=======以上为回路类=================================================================================




//=======以下为CToPolyline类=================================================================================

//将vector<CEdge*>转化成多义线类；
//vector<CEdge*>为已经排序好的路径path。但是需要识别CEdge之间的端点接驳顺序；
class CToPolyline
{
private:
	vector<CEdge*>* m_edgeLink; 
	AcDbPolyline* pLine;
	vector<AcDb2dVertex*> m_vertexArr;  //顶点装载于此;
	int m_startPointIndex;  //temp var; 用于记录取顶点过程中，某边的起点的顶点序号;
	
	bool m_isclosed;  //指示多义线是否闭合;
	int m_nNumOfEdges; //m_edgeLink有几条边;

public:
	CToPolyline(vector<CEdge*>*& pEdgeLink,IN const AcGeTol equalPointTol);
	~CToPolyline();
	CToPolyline();
	void inputEdgelink(vector<CEdge*>* pEdgeLink);
	void to_polyline(IN AcDbPolyline*& pLine);  //做成多义线; 

private:
	//对路径加工成一系列顶点AcDb2dVertex；
	void to_vertex2d();
	void getStartVertexIndex();   //获取第一个顶点的序号; 
	int getFirstVertexIndex();  //辨别起点的序号;
	int getLastVertexIndex();  //辨别终点顶点序号;
	void transEdgeToVetex(CEdge* pEdge);
	
	bool isPathClosed();  // 所有顶点都加入之后，判断该多义线是否闭合;
	// 若某条边CEdge是多义线（不闭合的），则需将多义线顶点加入新多义线。
	void insertPolyline(AcDbPolyline* pline,bool isReverse);   // isReverse表示是否需要将pline反过来加入新多义线；
	void insertLine(IN const CEdge* pEdge);
	void insertArc(IN const CEdge* pEdge);
	void insertPolyline(IN const CEdge* pEdge);
	//取弧的bugle；判断是否从弧的起点还是重点来计算bugle，方向不一样;
	double getArcBugle(AcDbArc* pArc, bool isFromStartpoint); 
	bool getLastVertex(); //取最后一个顶点，做成vertex;

	//获取第一个edge代表的第一个顶点;  //已作废;
	void getFirstPosition();  //已作废;
	void getFirstVertex(CEdge* pedgeFirst,CEdge* pedgeSecond); //已作废;
	void getNextVertex(CEdge* pedge);  //已作废;
	void getLastVertex(CEdge* pedge);  //已作废;
	void inputEquPntTol(IN const AcGeTol equalPointTol);   //已作废;
	//点坐标相等判断用误差值; //已作废;
	AcGeTol m_equalPoint;  //已作废;
	bool isVetexListClosed();  //已作废;

private:
	void printVertex(IN AcDb2dVertex*& pt);  //测试：打印顶点信息；   
	void printEdgeLink();   //打印边表信息;   
};
//=======以上为CToPolyline类=================================================================================



//=======以下为图类=================================================================================

//顶点;用CAD的顶点坐标为DATA；采用多重邻接表；
class CVertex
{
public:
	AcGePoint3d pt;    //autocad的点；
	int m_degree;  //度;
public:
	CEdge* adj;   //链接边;

public:
	CVertex(const AcGePoint3d& ptinput);	// 构造函数；
	CVertex();
	~CVertex();
	bool operator == (CVertex & v1);
	void getPoint(AcGePoint3d& ptretn);  // 返回顶点坐标；
};



//边类；边可能是line，arc，polyline之一；closed polyline不参与运算；
//多重链接表表示；(不是十字链表）；
class  CEdge
{
public:
	CEdge(AcDbEntity* pent);  //一个实体图元构造一条边;
	CEdge();
	~CEdge();

public:
	int index1;    //顶点1在图中的序号；
	int index2;     //顶点2在图中的序号；
	CEdge* path1;  //第一个顶点的下一个邻接边； 
	CEdge* path2;   //第二个顶点的下一个邻接边；  

	AcGePoint3d ptstart;  //线段起点；
	AcGePoint3d ptend;   //线段终点；
	CEdge* ptrSameEdges;  //同一条边可能有多个实体图元（line，arc，polyline）;
	AcDbEntity* ptrEnt;
	AcGe::EntityId enttype;   //表明实体是什么类：line，or arc，polyline。。。 
	//AcDbEntity* pEnt;   //指向一个实体；指针比较省空间。 

	int m_numSameEdges;   //此边（图结构）上有几个实体图元边，重叠的;
	int m_leftSameEdges;  //统计此条边被用了之后，还剩几条；若为0，则不能再用了；哪里用到这个参数？：提取环路; 

public:
	bool m_isJoinedIntoNewPolyline;  //标记此实体是否加入了新的多义线（完成了topl之后); 
	CEdge* getNextPathPtr(IN const int iIndex);  //根据输入序号，和index1和index2对比，决定返回path1还是path2;
	bool setNextPathPtr(IN const int iIndex,IN CEdge* pEdge);  //设置path1或path2的值;即改变path指针;
};



//根据实体集合确定最外框线及框线坐标，根据坐标数值级别，估算;   
//点坐标相等的误差值；
//
class CGetPointEqualAcGeTol
{
private:
	double m_tol;   //误差值;
public:
	CGetPointEqualAcGeTol();  
	~CGetPointEqualAcGeTol();  

public:
	AcGeTol getPointTolerance(IN ads_name ss,OUT int& nDotNum);  
};


//图类,存储所有实体;
//采用多重链表;
//顶点按int编号;
//算法修改：应该在遍历轮廓线之前提取“重复边”，否则会导致轮廓线破碎不堪;(当然这也是一种选择而已)
class  CGraphEnts    //所有的图形（line，arc，polyline）都在这里组成图结构;  
{
	//friend class CArcLink;
	//friend class CArcLinkArray;  //友元类，为了访问该类的私有成员，比如图结构; 
private:
	int MaxNumVertexs;    //最大顶点数;
	int numVertexs;      //目前实际顶点数;  
	vector<CVertex* > m_vertexTable;  //顶点表; 
	vector<CEdge*> m_vctEdges;  //边表；收集用户分配的内存，主要用来释放其内存。 
	int numEdges;         //图的边数;
	ads_name m_sel;      //用来聚合多义线的实体;过滤后才传入本类;    
	long m_ssLength;   // 实体集数量(实体边);
	CArcLink  m_stackEdges;  //深度遍历图过程中，存放遍历的边系列的栈;  
	CArcLinkArray m_allLoops;  //所有的可能的路径都装载于此; 

	vector<int> visited;    //记录节点是否被访问过;	

	CPointMap m_pointMap;  //存储点坐标;
	int m_nDotNum;  //点坐标小数位保留多少位.  

private:
	vector<CEdge*> m_paths;  //临时用作深度遍历后，从carclinkarray中取出路径（检查是否存在，真实的路径）；

public:
	CGraphEnts();
	~CGraphEnts();   //析构函数;  

public:
	void createGraph(); //造图;
	//对图进行改造，每个度大于2的顶点都拆开来，使得每个顶点度不大于2;
	//提取路径（闭合及不闭合）:a-重叠边先提取环路径;b-然后提取连续路径;  
	//对路径生成多义线;  
	void to_polyline(IN ads_name ssPick); 
	static bool isTwoPointsEqual(IN const AcGePoint3d& pt1,IN const AcGePoint3d& pt2,IN const int nDotNUm); 
	
private:
	int insertVertex(IN AcGePoint3d &pt);     //向图中插入一个顶点;    
	void insertEdge(CEdge* pedge);    //向图中插入一条边;   
	int delVertex(int v);  //删除图中一个顶点(即相关边也要删除);     
	bool delEdge(CEdge* pedge);   //删除图中一条边;  

private:
	int GetFirstNeighbor(int v1,OUT CEdge*& pedge);  //取顶点v1的第一个邻接顶点；
	int GetNextNeighbor(const int v1,const int v2,CEdge*& pedge);  //取顶点v1的在邻接顶点v2后的邻接顶点v3;
	int findVertexNode(const AcGePoint3d& pt);  //通过坐标点pt获得顶点位置;
	CEdge* findEdge(const AcGePoint3d& pt1,const AcGePoint3d& pt2); 
	CEdge* findEdge(IN const CEdge* pedge);  //pedge是还没插入图；但是已知其顶点序号及顶点；
	int getDegreeOfVertex(int index);  //获取某个顶点的度（就是入边和出边的数量和，无向图没有出边和入边之分，只有相关边）；
	void testIfLoopONextVtxNode(const int index);  /*,CArcLink& objCArcLink);*/ //测试所有该顶点的邻接点，确定是否形成环或者路径；

	bool createEdge(CEdge*& pedge,ads_name ssunit);  //根据实体ssunit建立一条边edge对象；被插入边函数调insertEdge（）用；
	void resetvisited();// 重新设置visited[];

	void  extractEdgeLinks(IN const int iIndex);  //每个顶点的度都是<=2，直接提取每条边表即可;
	//拆分边链表：出入度大于2的边表都要拆分，添加新顶点，拆分成多个边表;
	void chopEdgeLinks();
	int insertSameVertex(IN const int vtxIndex);  //插入新顶点，此顶点肯定是重复顶点(拆分出来的顶点);
	void chopVertex(IN IN const int iIndex);
	//取某顶点iParent的除v1外的另一顶点，前提是该顶点度为2;
	int getAnotherVertex(IN const int v1,IN const int iParent,IN CEdge*& pEdge); 
	bool isVertexIndexValid(IN const int iVertexIndex);   
	bool extractOverlappedEdges(); 	
	bool extractOverlappedEdge(IN CEdge* pEdge); //对指定边提取重叠边构成的环路;  

	//深度遍历不用递归方式，而是用栈结构，使用循环来遍历;   
	//index是某个顶点序号；若图是联通的，只遍历一次;  
	void  DFS(int index);    //深度遍历，获取边表; 
	void DFS2(int iIndex);  //因为所有顶点只有1-2度，所以只提取边表，不需要做深度遍历；
	//测试；
	void testDFS3(IN int iIndex);  //遍历图，输出边表; 仅仅测试用;  
	void DFS3();  //调用testDFS3，完成图遍历;测试；
	
private:
	void prsDotNumAndTol();  //分析点坐标小数位数量、获取误差值;

	//测试函数；打印边的信息；
	void static printEdgeInfo(CEdge* pEdge);
};


//在DFS过程中，stack保存了路径，1.若有回路路径，则需要提取；
//提取回路后，保存在stack中，然后做成闭合多一线；
//2.若不是回路，也将其连接成飞闭合多一线；

#endif  //ZHOUHUAGANG_20160612_TO_PL_DOT_H





/* 开发过程备忘录：topln；
2017-11-17 开发问题备忘录：
1-坐标点相等判定方法：
1.1 equal函数的误差值取得；
1.2 equal运用，需要统一使用一个误差值；
1.3 误差值要考虑图形大小及位置;

2。实体集选择函数要重新整理；

3.  轮廓线关系检查：1.相交；2.缺口；此为预先处理;
3.1 提示用户修改;

4.0
4.1如果只有两条重叠线的情况下，不会对其进行
做多义线； 错误：因为程序识别为2条线路；

5.0
5.1 生成多义线的层设置；

6.0
6.1 
*/