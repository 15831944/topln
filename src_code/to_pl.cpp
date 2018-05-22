/************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
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
#include "..\StdAfx.h"
#include  "to_pl.h"
#include "selectss.h"
#include "TCHAR.h"
//=============CEdge类========================================start=======
//构造函数
CEdge::CEdge()
{
	index1 = -1;    //顶点1在图中的序号；
	index2 = -1;     //顶点2在图中的序号；
	path1 = NULL;  //第一个顶点的下一个邻接边；
	path2 = NULL;   //第二个顶点的下一个邻接边；
	//AcGePoint3d ptstart;  //线段起点；
	//AcGePoint3d ptend;   //线段终点；
	ptrSameEdges = NULL;  //同一条边可能有多个实体图元（line，arc，polyline）；
	ptrEnt = NULL;
	//AcGe::EntityId enttype;   //表明实体是什么类：line，or arc，polyline。。。
	//AcDbEntity* pEnt;   //指向一个实体；指针比较省空间。

	int m_numSameEdges = 0;   //此边（图结构）上有几个实体图元边，重叠的； 
	m_leftSameEdges = 0;  //统计此条边被用了之后，还剩几条；若为0，则不能再用了；哪里用到这个参数？：提取环路；
}

//析构函数;
CEdge::~CEdge()
{
}
//=============CEdge类========================================end=======



//=============CVertex类========================================start=======
//构造函数
CVertex::CVertex()
{
}
//析构函数
CVertex::~CVertex()
{
}

//析构函数2
CVertex::CVertex(const AcGePoint3d& ptinput)
{
	pt.set(ptinput.x,ptinput.y,ptinput.z);
	adj = NULL;
}

//
void
CVertex::getPoint(AcGePoint3d& ptretn)
{
	ptretn.set(pt.x,pt.y,pt.z);
}
//=============CVertex类========================================end=======





//=============CArcLink类========================================start=======
//边集合类；
//构造函数；
CArcLink::CArcLink()
{
	isClosed = false;
}

//析构函数；
CArcLink::~CArcLink()
{
}



//判断此边链表是否闭合；
//前提：此路径不包含环（整理过了的）;
bool
CArcLink::isPathClosed()
{
	if(m_edgesLink.size() < 2)
	{
		return false;
	}

	int index1_one;
	int index2_one;
	int index1_two;
	int index2_two;//临时变量
	int index_last; //最后一条边的结尾顶点；
	int index_first; //第一条边的起始顶点；

	vector<CEdge*>::iterator itr = m_edgesLink.end();
	itr--; //回退一下，才是指向结尾节点；否则是结束；
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	//当路径只有2条边时
	if (m_edgesLink.size() == 2)
	{
		if((index1_one == index1_two) && (index2_one == index2_two))
		{
			return true;
		}
		if((index1_one == index2_two) && (index2_one == index1_two))
		{
			isClosed = true;
			return isClosed;
		}
		isClosed = false;
		return isClosed;
	}

	//先取得最后一条边的结尾顶点序号；
	//确定尾边尾顶点；
	if(index1_two == index1_one || index1_two == index2_one)
	{
		index_last = index2_two;
	}
	else if(index2_two == index1_one || index2_two == index2_one)
	{
		index_last = index1_two;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//===
	//计算第一个顶点的起始点；
	itr = m_edgesLink.begin();
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	itr++;
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	if(index1_one == index1_two || index1_one == index2_two)
	{
		index_first = index2_one;  //得到头结点序号。
	}
	else if(index2_one == index1_two || index2_one == index2_two)
	{
		index_first = index1_one;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//判断头结点是否和尾节点相同；
	CArcLink* pArcLink ;
	if(index_first != index_last)
	{
		isClosed= false;
		return isClosed;
	}
	else
	{
		isClosed = true;
		return isClosed;  //不用循环了.退出本函数；
	}
}




//设置：路径是否闭合参数
void
CArcLink::setClosed(bool isitclosed)
{
	isClosed  = isitclosed;
}



//添加CEdge对象。
void
CArcLink::push_back(CEdge* pedge)
{
	if(pedge !=  NULL)
	{
		m_edgesLink.push_back(pedge);
	}
	else
	{
		acutPrintf(_T("\nCArcLink::push_back(CEdge* pedge):pedge为空-error"));
	}
}

void
CArcLink::push_front(IN CEdge* pedge)
{
	if(pedge !=  NULL)
	{
		m_edgesLink.insert(m_edgesLink.begin(),pedge);
	}
	else
	{
		acutPrintf(_T("\nCArcLink::push_back(CEdge* pedge):pedge为空-error"));
	}
}


//取尾部元素；
CEdge*
CArcLink::back()
{
	CEdge* pEdge = NULL;
	if(m_edgesLink.size() > 0)
	{
		pEdge = m_edgesLink.back();
		return pEdge;
	}
	else
	{
		return NULL;
	}
}



//弹出尾部CEdge对象。
CEdge*
CArcLink::pop_edge()
{
	CEdge* pedge;
	if(m_edgesLink.size() > 0)
	{
		pedge = m_edgesLink.back();

		m_edgesLink.pop_back();
		return pedge;
	}
	return NULL;
}


//添加顶点顺序号；
void
CArcLink::push_back(int index)
{
	if (index != -1)
	{
		m_vectroIndex.push_back(index);
	}
}


//取edge数量；
int
CArcLink::sizeOfEdges()
{
	return m_edgesLink.size();
}


//清空
void
CArcLink::clear()
{
	m_edgesLink.clear();
}


//弹出尾部节点序号;
int
CArcLink::pop_index()
{
	int index;
	if(m_vectroIndex.size() > 0)
	{
		index = m_vectroIndex.back();

		m_vectroIndex.pop_back();

		return index;
	}
	return -1;  //返回-1表示错误
}




//对闭合路径进行整理；去掉非环部分的边；因为在图中提取环的时候，环可能带有尾巴；
//尾巴去掉后，建立新的CArcLink,重新加入CArcLinkArray？
void
CArcLink::correctLinks(CArcLinkArray* pLinkArr)
{
	if(m_edgesLink.size() <= 2  || isClosed == false)  //检查路径边数以及是否闭合；
	{
		return;
	}

	int index1_one;
	int index2_one;
	int index1_two;
	int index2_two;//临时变量
	int index_last; //最后一条边的结尾顶点；
	int index_first; //第一条边的起始顶点；

	//每条边和结尾边结尾顶点序号进行比较，检查是否闭合；否则删除之；
	//先取得最后一条边的结尾顶点序号；
	vector<CEdge*>::iterator itr = m_edgesLink.end();
	itr--; //退回一步，否则itr是没有意义的。understand？
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	//确定最后的顶点；
	if(index1_two == index1_one || index1_two == index2_one)
	{
		index_last = index2_two;
	}
	else if(index2_two == index1_one || index2_two == index2_one)
	{
		index_last = index1_two;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//===
	//计算第一个顶点的起始点；
	itr = m_edgesLink.begin();
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	itr++;
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	if(index1_one == index1_two || index1_one == index2_two)
	{
		index_first = index2_one;  //得到头结点序号。
	}
	else if(index2_one == index1_two || index2_one == index2_two)
	{
		index_first = index1_one;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//判断头结点是否和尾节点相同；
	CArcLink pArcLink ;
	if(index_first != index_last)
	{
		//pArcLink = new CArcLink();
		pArcLink.push_back((CEdge*)(*itr));
		itr = m_edgesLink.erase(itr); //删除当前边，itr指向下一条边；
	}
	else
	{
		return;  //不用循环了.退出本函数；
	}

	//继续去掉非闭合的部分；
	for(; itr < m_edgesLink.end(); itr++)
	{
		//计算第一个顶点的起始点；
		index1_one = (*itr)->index1;
		index2_one = (*itr)->index2;
		itr++;
		index1_two = (*itr)->index1;
		index2_two = (*itr)->index2;
		itr--;
		if(index1_one == index1_two || index1_one == index2_two)
		{
			index_first = index2_one;  //得到头结点序号。
		}
		else if(index2_one == index1_two || index2_one == index2_two)
		{
			index_first = index1_one;
		}
		else
		{
			acutPrintf(_T("CArcLink::correctLinks() error log"));
		}

		//判断头结点是否和尾节点相同；
		if(index_first != index_last)
		{
			pArcLink.push_back((CEdge*)(*itr));
			itr = m_edgesLink.erase(itr); //删除当前边，itr指向下一条边；
		}
		else
		{
			break;  //不用循环了.
		}
	}
	pLinkArr->push_back(pArcLink);
}



//根据实际的边实体图形来决定某条路径是否应该：1.>=1,则重复生成；2.==0则分裂成多条边（真实的）。
//此函数功能即是完成path调整做准备。生成能实际生成多义线的path；
//还计算多重叠边的重叠数；
void  
CArcLink::numOfLoops()
{
	vector<CEdge* >::iterator itr = m_edgesLink.begin();
	int minx =  (*itr)->m_leftSameEdges;
	int maxx = minx;
	int i = 0;
	for(; itr != m_edgesLink.end(); itr++)
	{
		i = (*itr)->m_leftSameEdges;
		if(i < minx)
		{
			minx  = i;
		}

		if(i > maxx)
		{
			maxx = i;
		}
	}
	m_numOfLoops = minx;	
	acutPrintf(_T("\n最小重叠层数为%d"),m_numOfLoops);
}


//提取closed path；每次提取一环；
//此函数已经假定有环可以提取；
//一条边CEdge提取一次后，需重新设置其参数；
//每次提取CEDGE都是从链表最后节点提取；链表节点数量是在变的；链表节点并不变；
void 
CArcLink::extractLoops(vector<vector<CEdge*>> & pEdgesLinks)
{
	//if(isClosed == false)  //先检查是否闭合；//不闭合也要提取重叠线路2017-11-20;
	//{
	//	return;
	//}

	CEdge* pEdge; //
	vector<CEdge*> pVectEdges;

	vector<CEdge* >::iterator itr; /*= m_edgesLink.begin(); */
	while(m_numOfLoops > 0)
	{
		for(itr = m_edgesLink.begin(); itr < m_edgesLink.end();  itr++)
		{
			//检查pEdge剩下值，挑选相应pEdge;因为pEdge有重叠;
			//找到最后一个pedge；
			int i;
			//pEdge = (*itr)->ptrSameEdges;
			pEdge = (CEdge*)(*itr);
			for(i = 1; i < (*itr)->m_leftSameEdges; i++)  //从 1开始;总是取重叠边中最后一个边;
			{
				pEdge = pEdge->ptrSameEdges; //深入；
			}
			pVectEdges.push_back(pEdge);

			//CEdge被提取后，自身参数相应修改；
			//修改剩余CEdge；
			(*itr)->m_leftSameEdges--;			
		}
		pEdgesLinks.push_back(pVectEdges);
		pVectEdges.clear();

		m_numOfLoops--;  //最终变为0；
	}
	//以上。 然后提出单个pedge重叠的边构成的环（2边环）；	
}



//提取整个路径上的环后，再提取每条边上的环（因为边可能是重叠边）；
//对此边重叠边成对提取；每对（2个）边是一个环；
//注意CEdge上的各种参数的变动；
//此处先计算剩余是奇数还是偶数；然后决定是否跳过第1个边；然后成对提取；
void
CArcLink::extractLoopsOfOverlappedEdges(OUT vector<vector<CEdge*>> & pEdgeLinks)
{
	CEdge* pEdge;
	vector<CEdge*> pVectEdge;

	int edgeDepth = 0;	
	vector<CEdge*>::iterator itr = this->m_edgesLink.begin();
	for(; itr < m_edgesLink.end(); itr++)
	{		
		pEdge = (CEdge*)(*itr);
		CEdge* pEdgeTemp = pEdge;
		edgeDepth = pEdge->m_leftSameEdges;
		if(edgeDepth > 1)  //此边重叠数超过1，则需将重叠边每两个一组取出放于pEdgeLinks；
		{
			int i = 0;
			i = edgeDepth % 2;   //测试奇偶;
			if(1 == i)
			{
				pEdgeTemp = pEdgeTemp->ptrSameEdges;
			}
			while(pEdgeTemp)
			{
				pVectEdge.push_back(pEdgeTemp);
				pEdgeTemp = pEdgeTemp->ptrSameEdges;
				pVectEdge.push_back(pEdgeTemp);
				pEdgeTemp = pEdgeTemp->ptrSameEdges;  //bug

				pEdgeLinks.push_back(pVectEdge); 
				pVectEdge.clear();
				pEdge->m_leftSameEdges -= 2;   //此边剩余数量每次减去2，最后可能是0或者1；
			}
		}
	}
}


//提取not closed path；；
//前提:重叠边已经被提取过了;
//其实每条边剩余重叠边不是0就是1；只能提取一次了；
//根据边的重叠数量提取边链表：m_leftSameEdges；
//
void 
CArcLink::extractPathNotClosed(vector<vector<CEdge*>> & pEdgeLinks)
{
	//定义一个变量，收集所有轮廓线;
	vector<vector<CEdge*>> vEdgeLists;
	vEdgeLists.empty();

	CEdge* pEdge; 
	vector<CEdge*> pVctEdge;
	vector<CEdge*>::iterator  itr = this->m_edgesLink.begin();
	for(; itr < m_edgesLink.end(); itr++)   //一次遍历整个path；
	{
		pEdge = (CEdge*)(*itr);
		if(pEdge->m_leftSameEdges != 0)
		{
			pVctEdge.push_back(pEdge);  
			pEdge->m_leftSameEdges--;    
		}     
		else
		{
			if(pVctEdge.size() != 0)
			{
				vEdgeLists.push_back(pVctEdge);
				pVctEdge.clear();
			}
		}
	}

	//判断pVctEdge是否为空；不为空则需要加入；最后一段边链表特殊处理于此;
	if(pVctEdge.size() != 0)
	{
		vEdgeLists.push_back(pVctEdge);
		pVctEdge.clear();
	}

	//提取的线路，加入pEdgeLinks
	vector<vector<CEdge*>>::iterator itrlist = vEdgeLists.begin();
	for(; itrlist != vEdgeLists.end(); itrlist++)
	{
		pEdgeLinks.push_back((vector<CEdge*>)(*itrlist));
	}
}


//判断某条边是否重叠数为奇数;
//奇数说明此边提取重叠边后还剩下一条边;
bool
CArcLink::isEdgeLeftAfterLoopsExtacted(const CEdge* pEdge)
{
	int i = 0;
	i = pEdge->m_numSameEdges;
	int j = 0;
	j = i % 2;
	if(j == 1)
	{
		return true;
	}
	return false;
}


//获取arclink第一段的起始点序号.
//取第一段，第二段，对比相同顶点，得出起始点序号;
bool 
CArcLink::getStartPointIndex(OUT int& iIndex) 
{
	iIndex = -1;

	//序号
	int a1 = 0; //第一段index1;
	int a2 = 0;
	int b1 = 0;
	int b2 = 0;
	
	int nNumEdges = 0;
	nNumEdges = m_edgesLink.size(); 
	if(nNumEdges == 1)
	{
		a1 = m_edgesLink[0]->index1;
		a2 = m_edgesLink[0]->index2;
		iIndex = a1;
		return true;
	}
	else if(nNumEdges >= 2)
	{
		a1 = m_edgesLink[0]->index1;
		a2 = m_edgesLink[0]->index2;
		b1 = m_edgesLink[1]->index1;
		b2 = m_edgesLink[1]->index2;
		if(a1 == b1 || a1 == b2)
		{
			iIndex = a2;
			return true;
		}
		else if(a2 == b1 || a2 == b2)
		{
			iIndex = a1;
			return true;
		}
	}
	//iIndex = -1;
	return false;
}


//获取arclink最后一段段的终点序号.
//取倒数第一段，倒数第二段，对比相同顶点，得出起始点序号;
//返回false，代表失败了;
bool
CArcLink::getEndPointIndex(OUT int& iIndex)
{
	iIndex = -1; //initialize

	//序号
	int a1 = 0; //倒数第一段index1;
	int a2 = 0;
	int b1 = 0;
	int b2 = 0;

	int nNumEdges = 0;
	nNumEdges = m_edgesLink.size(); 
	if(nNumEdges == 1)
	{
		a1 = m_edgesLink[nNumEdges -1]->index1; 
		a2 = m_edgesLink[nNumEdges -1]->index2; 
		iIndex = a2;
		return true;
	}
	else if(nNumEdges >= 2)
	{
		a1 = m_edgesLink[nNumEdges -1]->index1; 
		a2 = m_edgesLink[nNumEdges -1]->index2; 
		b1 = m_edgesLink[nNumEdges -2]->index1;
		b2 = m_edgesLink[nNumEdges -2]->index2;
		if(a1 == b1 || a1 == b2)
		{
			iIndex = a2;
			return true;
		}
		else if(a2 == b1 || a2 == b2)
		{
			iIndex = a1;
			return true;
		}
	}
	//iIndex = -1;
	return false;
}


//测试arclink第一段和最后一段是否闭合（提取了重叠边之后）
//step1：测试第一段和最后一段是否为空（提取重叠边）后；
//step2：测试起点和终点是否重合（轮廓线是否闭合);
//返回：true：闭合； false：不闭合;
bool
CArcLink::isFirstAndLastEdgeLinked()
{
	
	CEdge* pEdge = NULL;
	int nNumOfEdges = 0;
	nNumOfEdges = m_edgesLink.size();  
	if(nNumOfEdges <= 0)
	{
		return false;
	}

	//判断第一段在提取重叠边后，是否还有剩余边;
	pEdge = m_edgesLink[0];
	if(!isEdgeLeftAfterLoopsExtacted(pEdge))
	{
		return false;
	}

	pEdge = m_edgesLink[nNumOfEdges-1];
	if(!isEdgeLeftAfterLoopsExtacted(pEdge))
	{
		return false;
	}

	//起点序号
	int nStartIndex = -1;
	if(!getStartPointIndex(nStartIndex))
	{
		return false;
	}
	//终点序号
	int nEndIndex = -1;
	if(!getEndPointIndex(nEndIndex))
	{
		return false;
	}
	//比较;
	if(nStartIndex == nEndIndex)
	{
		return true;
	}
	return false;
}


//若arclink第一段和最后一段是连续的，合并之;
//输入：vector<vector<CEdge*>> & pEdgesLinks 已经提取好的各个轮廓线;
//*pEdgesLinks不能含有重叠边构成的轮廓线;否则出错;
//返回：false没法合并;
bool
CArcLink::mergeFirstAndLastLink(vector<vector<CEdge*>> & pEdgesLinks)
{
	int nNum = 0;
	nNum = pEdgesLinks.size();
	if(nNum <= 1)
	{
		return false;
	}

	if(!isFirstAndLastEdgeLinked())
	{
		return false;
	}

	//merge first and last; add the first to the end;
	CEdge* pEdge = NULL;
	vector<CEdge*>* pEdgeListFirst = NULL;  
	vector<CEdge*>* pEdgeListEnd = NULL; 
	vector<vector<CEdge*>>::iterator itr = pEdgesLinks.begin();  
	pEdgeListFirst = &((vector<CEdge*>)(*itr));
	itr = pEdgesLinks.end();
	itr--;
	pEdgeListEnd = &((vector<CEdge*>)(*itr)); 
	//add the last to the first 
	vector<CEdge*>::iterator itrEdge = pEdgeListFirst->end();
	for(; itrEdge != pEdgeListFirst->end(); itrEdge++)
	{
		pEdgeListEnd->push_back((CEdge*)(*itrEdge)); 
	}
	//erase the first;
	pEdgesLinks.erase(pEdgesLinks.begin());
	return true;
}


//测试用的函数；测试完可删除; 
void 
CArcLink::testPintOriginalPath() //打印从图里面深度遍历后得到的路径;  
{
#ifdef DEBUG_TO_PL_PRINT
	//CEdge* pEdge;
	acutPrintf(_T("路径开始：\n"));
	vector<CEdge*>::iterator itr = m_edgesLink.begin();
	for(; itr < m_edgesLink.end(); itr++)
	{
		acutPrintf(_T("顶点：(%d,%d)\n"),(*itr)->index1,(*itr)->index2);
	}

	acutPrintf(_T("路径结束！\n"));
#endif //DEBUG_TO_PL_PRINT
}

//=============CArcLink类========================end================;






//=============CArcLinkArray类========================================start=======
//根据实际的边实体图形来决定某条路径是否应该：1.重复生成；2.分裂成多条边（真实的）；等等。
//此函数功能即是完成path调整。生成能否实际生成多义线的path；

//构造函数
CArcLinkArray::CArcLinkArray()
{
	;
}


CArcLinkArray::~CArcLinkArray()
{
	;
}


//整理出真实的路径；
//先提取环路； 如果还有剩下的边，提取非环路；
bool
CArcLinkArray::correctByRealEdges()
{
	vector<CArcLink>::iterator itr = this->m_edgeLinkArray.begin();	

	//对carclink进行整理，去掉closed path中非环部分进行剥离;
	//for(; itr < m_edgeLinkArray.end(); itr++)
	//{
	//	if((*itr).isPathClosed())
	//	{
	//		(*itr).correctLinks(this);
	//	}
	//}

	//非环路提取;
	for(itr = m_edgeLinkArray.begin(); itr < m_edgeLinkArray.end(); itr++)
	{
		//(*itr).numOfLoops(); //获取carclink内部真实loop数量；因为carclink的CEdge可能已经变动了；
		//(*itr).extractLoops(m_edgeLinkArrayToPolyline);
		(*itr).extractLoopsOfOverlappedEdges(m_edgeLinkArrayToPolyline);
		(*itr).extractPathNotClosed(m_edgeLinkArrayToPolyline);
	}
	return true;
}


//在尾巴插入边链表；
bool
CArcLinkArray::push_back(CArcLink edgesLink)
{
	int num = edgesLink.sizeOfEdges();
	if(num != 0)
	{
		m_edgeLinkArray.push_back(edgesLink);
		return true;
	}
	return false;
}


//弹出尾巴边链表元素；
void
CArcLinkArray::pop_back()
{
	//	CArcLink* pArcLink;
	if(m_edgeLinkArray.size() != 0)
	{
		m_edgeLinkArray.pop_back();
	}
	return ;
}


////对环形路径（closed path）进行提取；优先提取； 对所有carclink循环一遍；
////环形closed path 提取后，其余arclink再进行提取path； 再对所有carclink循环一遍；
//void
//CArcLinkArray::extractLoops()
//{
//	vector<CArcLink*>::iterator itr = this->m_edgeLinkArray.begin();
//	for(; itr < m_edgeLinkArray.end(); itr++)
//	{
//		itr->numOfLoops();
//		itr->extractLoops(m_edgeLinkArrayToPolyline);  //提取环；若没有环，则不会提取；
//	}
//
//	for(; itr < m_edgeLinkArray.end(); itr++)
//	{
//		itr->numOfLoops();
//		itr->extractPathNotClosed(m_edgeLinkArrayToPolyline);  //提取非环路径；若没有路径，则不会提取；
//	}
//}


////根据
////生成多义线；
//void
//CArcLinkArray::toPolylines(vector<CEdge*> & pVecEdges)
//{
//	int i = pVecEdges.size();
//	AcDbPolyline* pline = new AcDbPolyline();  
//
//	vector<CEdge*>::iterator itr = pVecEdges.begin(); 
//
//	CEdge* p1 = (CEdge*)itr;
//	itr++;
//	CEdge* p2 = (CEdge*)itr;
//
//	
//	;
//}


////通过CEdge p1，p2获取第一个顶点的vertex值；
////求p1代表的起点的vertex；五要素：point，startwidth，endwidth，bugle，index；
//void
//CArcLinkArray::getVertex(CEdge* p1,CEdge* p2)
//{
//	AcGePoint3d pt1(p1->ptstart);
//	AcGePoint3d pt2;
//	AcGePoint3d pt3;
//	AcGePoint3d pt4;
//
//	AcGePoint3d ptFirst;
//	AcGePoint3d ptSecond;
//
//	;
//}


//路径整理好了之后，将路径做成多义线；
void
CArcLinkArray::toPolylines()
{
	CToPolyline objToPl;
	vector<CEdge*>* pedgelink;
	vector<vector<CEdge*>>::iterator itr = m_edgeLinkArrayToPolyline.begin();
	for(;itr < m_edgeLinkArrayToPolyline.end(); itr++)
	{
		AcDbPolyline* pline = new AcDbPolyline();

		objToPl.inputEdgelink((vector<CEdge*> *)(&(*itr))); //传入参数; 
		objToPl.to_polyline(pline);
		//m_vecPolylines.push_back(pline);
		join2database(pline);  //加入数据库；不收集起来了;

		pline->close();
	}	
}


AcDbObjectId
CArcLinkArray::join2database(AcDbEntity *pEnt)
{
	AcDbObjectId objid;
	AcDbBlockTable *pblktbl;
	AcDbBlockTableRecord *pblktblrcd;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pblktbl,AcDb::kForRead);
	pblktbl->getAt(ACDB_MODEL_SPACE,pblktblrcd,AcDb::kForWrite);
	pblktbl->close();
	//pEnt->upgradeOpen();
	pblktblrcd->appendAcDbEntity(objid,pEnt);
	//pEnt->downgradeOpen();
	pblktblrcd->close();
	return objid;
}


//测试函数；打印路径；
void
CArcLinkArray::testPrintOriginalPath()
{
#ifdef DEBUG_TO_PL_PRINT
	acutPrintf(_T("\nHere is CArcLinkArray::testPrintOriginalPath()---start:"));
	int iNum = 0; 
	vector<CArcLink>::iterator itr = m_edgeLinkArray.begin();
	for(; itr != m_edgeLinkArray.end(); itr++)
	{
		acutPrintf(_T("\n第%d条路径：\n"),iNum);
		(*itr).testPintOriginalPath();
		iNum++;
	}
	acutPrintf(_T("\nHere is CArcLinkArray::testPrintOriginalPath()---end!"));

#endif  //#ifdef DEBUG_TO_PL_PRINT
}


//测试打印用来生成多义线的m_edgeLinkArrayToPolyline内容；
void 
CArcLinkArray::testm_edgeLinkArrayToPolyline()
{
#ifdef DEBUG_TO_PL_PRINT
	int i = 0;
	int inum = m_edgeLinkArrayToPolyline.size();
	acutPrintf(_T("\nCArcLinkArray::testm_edgeLinkArrayToPolyline():获取了%d条用来生成多义线的真实路径。"),inum);
	vector<vector<CEdge*>>::iterator itrA = m_edgeLinkArrayToPolyline.begin();
	vector<CEdge*>::iterator itrB;
	for(; itrA < m_edgeLinkArrayToPolyline.end(); itrA++)
	{
		acutPrintf(_T("第%d条用来生成多义线的真实路径."),i);
		itrB = (*itrA).begin();
		for(; itrB < (*itrA).end(); itrB++)
		{
			acutPrintf(_T("\n顶点序号(%d,%d)"),(*itrB)->index1,(*itrB)->index2);
		}
		i++;
	}
#endif //#ifdef DEBUG_TO_PL_PRINT
}


//=============CArcLinkArray类========================end================;



//=============CToPolyline类========================start================;
//构造函数
CToPolyline::CToPolyline(vector<CEdge*>*& pEdgeLink,IN const AcGeTol equalPointTol)
{
	m_isclosed = false;

	if(pEdgeLink != NULL)
	{
		m_edgeLink = pEdgeLink;
	}

	m_equalPoint = equalPointTol;
}


//构造函数2
CToPolyline::CToPolyline()
{
	m_isclosed = false;
	m_startPointIndex = -1;  //随便初始化一下即可;
}


//析构函数
CToPolyline::~CToPolyline()
{	
}


//输入路径（用来生成多义线的)
void
CToPolyline::inputEdgelink(vector<CEdge*>* pEdgeLink)
{
	if(pEdgeLink != NULL)
	{
		m_edgeLink = pEdgeLink;
		printEdgeLink();  //(m_edgeLink);
	}
	else
	{
		acutPrintf(_T("CToPolyline::inputEdgelink() error!"));
	}
}


//输入误差值；
void
CToPolyline::inputEquPntTol(IN const AcGeTol equalPointTol)
{
	m_equalPoint = equalPointTol;
}



//顶点表完成后，判断是否闭合;
bool
CToPolyline::isVetexListClosed()
{
	//取顶点表的第一个顶点，及最后一个顶点;
	if(m_vertexArr.size() <= 1)
	{
		return false;
	}

	AcGePoint3d ptstr;
	AcGePoint3d ptend;

	AcDb2dVertex* ptrVtx2d ;
	
	ptrVtx2d = (AcDb2dVertex*)(m_vertexArr.front());
	ptstr = ptrVtx2d->position();
	ptrVtx2d = (AcDb2dVertex*)(m_vertexArr.back());
	ptend = ptrVtx2d->position();

	if(ptstr.isEqualTo(ptend))
	{
		return true;
	}

	return false;
}


//从边表获取第一个顶点;
void 
CToPolyline::getFirstPosition()
{
	AcDb2dVertex* pVertex2d;

	//取第一个edge
	CEdge* pEdgeFirst;
	CEdge* pEdgeSecond;
	vector<CEdge*>::iterator itr = m_edgeLink->begin();

	if(m_edgeLink->size() <= 0)
	{
		return;
	}
	else if(m_edgeLink->size() == 1)
	{
		pEdgeFirst =(CEdge*)(* itr);
		pVertex2d = new AcDb2dVertex();
		pVertex2d->setPosition(pEdgeFirst->ptstart);
		m_vertexArr.push_back(pVertex2d);  //第一个顶点；
		printVertex(pVertex2d);
		return;
	}

	//else if(m_edgeLink->size() > 1) 
	pEdgeFirst = (CEdge*)(* itr);
	itr++;
	pEdgeSecond = (CEdge*)(* itr);

	AcGePoint3d pt1start(pEdgeFirst->ptstart);
	AcGePoint3d pt1end(pEdgeFirst->ptend);
	AcGePoint3d pt2start(pEdgeSecond->ptstart);
	AcGePoint3d pt2end(pEdgeSecond->ptend);

	AcGePoint3d pt1;
	AcGePoint3d pt2;
	

	//求两个CEdge的连接点；	
	if(pt1start.isEqualTo(pt2start) || pt1start.isEqualTo(pt2end))
	{//pt1start是连接点；则第一个顶点是pt1end;
		if(pEdgeFirst->enttype == AcGe::kLine2d)
		{
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			m_vertexArr.push_back(pVertex2d);  //第一个顶点；
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //圆弧；
		{
			//第一个点；
			AcDbArc* pArc = (AcDbArc*)pEdgeFirst->ptrEnt;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(getArcBugle(pArc,false));
			m_vertexArr.push_back(pVertex2d); 
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kPolyline2d)
		{
			//加入第一个顶点；
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);		
		}
		else
		{
			acutPrintf(_T("CToPolyline class::getFirstVertex()001"));
		}
	}
	else if(pt1end.isEqualTo(pt2start) || pt1end.isEqualTo(pt2end))
	{
		if(pEdgeFirst->enttype == AcGe::kLine2d)
		{
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			m_vertexArr.push_back(pVertex2d);  //第一个顶点；		
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //圆弧；
		{
			//第一个点；
			AcDbArc* pArc = (AcDbArc*)pEdgeFirst->ptrEnt;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(getArcBugle(pArc,true));
			m_vertexArr.push_back(pVertex2d);  
			printVertex(pVertex2d);		
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kPolyline2d)
		{
			//加入第一个顶点；
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);		
		}
		else
		{
			acutPrintf(_T("CToPolyline class::getFirstVertex()002"));
		}
	}
	else
	{
		acutPrintf(_T("ERROR：CToPolyline class::getFirstVertex()003"));
	}
}


//取得第一个顶点的序号; 
//如果路径闭合，则此顶点是假定的第一个顶点;
//比较2个edge的序号接驳关系，从而确定起始点;
//m_startPointIndex不断更新;
void
CToPolyline::getStartVertexIndex()
{
	int index1 = -2;
	int index2 = -2;
	int index3 = -2;
	int index4 = -2;
	CEdge* pEdgeFirst = NULL;
	CEdge* pEdgeSecond = NULL;
	if(m_edgeLink->size() >1)
	{
		pEdgeFirst = (*m_edgeLink)[0];
		pEdgeSecond = (*m_edgeLink)[1];
		index1 = pEdgeFirst->index1;
		index2 = pEdgeFirst->index2;
		index3 = pEdgeSecond->index1;
		index4 = pEdgeSecond->index2;
		if(index1 == index3 || index1 == index4)
		{
			m_startPointIndex = index2;			
		}
		else if(index2 == index3 || index2 == index4)
		{
			m_startPointIndex = index1;
		}
		else
		{
			acutPrintf(_T("\n出错了.......getFirstVertexIndex()-001"));			
		}
	}
	else if(m_edgeLink->size() == 1)  //只有一条边，随便指定一个顶点为起点;
	{
		pEdgeFirst = (*m_edgeLink)[0];	
		m_startPointIndex = pEdgeFirst->index1;			
	}
	else
	{
		//如果出错了，说明异常严重.
		acutPrintf(_T("\n出错了.......getFirstVertexIndex()-002"));	
	}
}


//取第一个顶点的序号; 
//当边等于1，必须取index1，这是约定！ 因为取末端点必须取index2。
//当边数等于2，若不闭合，则结果准确; 若闭合，无需make最后的vertex；
//当边数大于2，结果准确;
int
CToPolyline::getFirstVertexIndex()
{
	int index1 = -1;  //initialize.
	int index2 = -1;
	int index3 = -1;
	int index4 = -1;
	CEdge* pEdgeFirst = NULL;
	CEdge* pEdgeSecond = NULL;
	int nNum = m_edgeLink->size();
	if(nNum > 1)
	{
		pEdgeFirst = (*m_edgeLink)[0];
		pEdgeSecond = (*m_edgeLink)[1];
		index1 = pEdgeFirst->index1;
		index2 = pEdgeFirst->index2;
		index3 = pEdgeSecond->index1;
		index4 = pEdgeSecond->index2;
		if(index1 == index3 || index1 == index4)
		{
			if(index2 == index3 || index2 == index4)  //说明这2段弧闭合了;
			{
				return -1;
			}
			else
			{
				return index2;
			}			
		}	
		else
		{
			return index1;
		}
	}
	else if(1 == nNum)
	{
		pEdgeFirst = (*m_edgeLink)[0];
		return pEdgeFirst->index1;
	}
	else  //出错了!
	{
		return -1;	
	}
}


//取最后一个顶点的序号
//当边等于1，最后一个端点序号取index2，这是约定！第一个端点取index1。
//当边数等于2，若不闭合，则结果准确; 若闭合，不需要最后的端点序号;
//当边数大于2，结果准确;
int
CToPolyline::getLastVertexIndex()
{
	int index1 = -2;
	int index2 = -2;
	int index3 = -2;
	int index4 = -2;
	CEdge* pEdgeLast = NULL;
	CEdge* pEdgeSecond = NULL;
	int nNum = m_edgeLink->size();
	
	if(nNum > 1)
	{
		pEdgeLast = (*m_edgeLink)[nNum-1];
		pEdgeSecond = (*m_edgeLink)[nNum-2];
		index1 = pEdgeLast->index1;
		index2 = pEdgeLast->index2;
		index3 = pEdgeSecond->index1;
		index4 = pEdgeSecond->index2;
		if(index1 == index3 || index1 == index4)
		{
			if(index2 == index3 || index2 == index4)  //说明这2段弧闭合了;
			{
				return -1;
			}
			else
			{
				return index2;
			}			
		}
		else
		{
			return index1;
		}
	}
	else if(1 == nNum)
	{
		pEdgeLast = (*m_edgeLink)[nNum-1];
		return pEdgeLast->index2;
	}
	else  //出错了！
	{
		return -1;	
	}
}


//对一条直线边提取顶点vertex；
void
CToPolyline::insertLine(IN const CEdge* pEdge)
{
	AcGePoint3d ptStart;
	//判断起点;
	if(pEdge->index1 == m_startPointIndex)
	{
		ptStart = pEdge->ptstart;
		m_startPointIndex = pEdge->index2;  //更新起点序号;
	}
	else if(pEdge->index2 == m_startPointIndex)
	{
		ptStart = pEdge->ptend;
		m_startPointIndex = pEdge->index1; //更新起点序号;
	}
	
	//添加vertex;
	AcDb2dVertex* vtxNow = new AcDb2dVertex();
	vtxNow->setPosition(ptStart); 
	vtxNow->setBulge(0);
	m_vertexArr.push_back(vtxNow);
	printVertex(vtxNow); //打印顶点信息,测试用;
}


//对弧段实体提取顶点
void
CToPolyline::insertArc(IN const CEdge* pEdge)
{
	AcGePoint3d ptStart;
	bool isUnClockwise = true; //决定弧加入的时候，bugle是否要取反;
	double dblBugle = 0;
	//判断起点;
	if(pEdge->index1 == m_startPointIndex)
	{
		ptStart = pEdge->ptstart;
		isUnClockwise = true;
		m_startPointIndex = pEdge->index2;  //更新起点序号;
	}
	else if(pEdge->index2 == m_startPointIndex)
	{
		isUnClockwise = false;
		ptStart = pEdge->ptend;
		m_startPointIndex = pEdge->index1; //更新起点序号;
	}

	//添加vertex;
	AcDb2dVertex* vtxNow = new AcDb2dVertex();
	vtxNow->setPosition(ptStart); 
	dblBugle = getArcBugle((AcDbArc*)(pEdge->ptrEnt),isUnClockwise);
	vtxNow->setBulge(dblBugle);
	m_vertexArr.push_back(vtxNow);
	printVertex(vtxNow); //打印顶点信息,测试用;
}


//边是一条多义线，对其提取顶点;
void
CToPolyline::insertPolyline(IN const CEdge* pEdge)
{
	bool isUnClockwise = true; //决定弧加入的时候，bugle是否要取反;

	//判断起点;
	if(pEdge->index1 == m_startPointIndex)
	{
		isUnClockwise = true;
		m_startPointIndex = pEdge->index2;  //更新起点序号;
		insertPolyline((AcDbPolyline*)(pEdge->ptrEnt),isUnClockwise);
	}
	else if(pEdge->index2 == m_startPointIndex)
	{
		isUnClockwise = false;
		m_startPointIndex = pEdge->index1; //更新起点序号;
		insertPolyline((AcDbPolyline*)(pEdge->ptrEnt),isUnClockwise);
	}
	else
	{
		acutPrintf(_T("\n出错了.......insertPolyline()-001"));
		return;
	}
}


//根据边表中的边计算多义线顶点;
void
CToPolyline::transEdgeToVetex(CEdge* pEdge)
{
	if(pEdge->enttype == AcGe::kLine2d)
	{
		insertLine(pEdge);
	}
	else if(pEdge->enttype == AcGe::kCircArc2d)
	{
		insertArc(pEdge);
	}
	else if(pEdge->enttype == AcGe::kPolyline2d)
	{
		insertPolyline(pEdge); //按多义线原有顺序插入。
	}
	else
	{
		acutPrintf(_T("ERROR：CToPolyline class::getNextVertex()003"));
	}	
}


//1.如果两条边自成环形。怎么处理？
//2.如果两条边不成环形； 怎么处理？
//3.最后一条边的结束顶点如何处理；
//答：每次获取2个顶点，第一个顶点是完整的AcDb2dVertex；第二个没有计算bugle值，用来指示下一条边的连接点，然后计算bugle值；
//    最后一个顶点需要特别处理，需要判断是否多义线闭合；
void 
CToPolyline::getFirstVertex(CEdge* pEdgeFirst,CEdge* pEdgeSecond)
{
	AcGePoint3d pt1start(pEdgeFirst->ptstart);
	AcGePoint3d pt1end(pEdgeFirst->ptend);
	AcGePoint3d pt2start(pEdgeSecond->ptstart);
	AcGePoint3d pt2end(pEdgeSecond->ptend);

	AcGePoint3d pt1;
	AcGePoint3d pt2;
	AcDb2dVertex* pVertex2d; 

	//求两个CEdge的连接点；	
	if(pt1start.isEqualTo(pt2start) || pt1start.isEqualTo(pt2end)) 
	{//pt1start是连接点；则第一个顶点是pt1end;
		if(pEdgeFirst->enttype == AcGe::kLine2d)
		{
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			//vtx2d.setBulge(0);
			//vtx2d.setStartWidth(0);
			//vtx2d.setEndWidth(0);
			//vtx2d.setTangent(0);		
			m_vertexArr.push_back(pVertex2d);  //第一个顶点；
			//第二个顶点;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //圆弧；
		{
			//第一个点；
			AcDbArc* pArc = (AcDbArc*)pEdgeFirst->ptrEnt;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(getArcBugle(pArc,false));
			m_vertexArr.push_back(pVertex2d); 
			//第二个点。
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kPolyline2d)
		{
			//加入第一个顶点；
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
			
			//加入第二个顶点；
			AcDbPolyline* pline = (AcDbPolyline*)(pEdgeFirst->ptrEnt);
			insertPolyline(pline,true);  //依照ptrEnt原来的顶点顺序，插入新多义线；
		}
		else
		{
			acutPrintf(_T("CToPolyline class::getFirstVertex()001"));
		}
	}
	else if(pt1end.isEqualTo(pt2start) || pt1end.isEqualTo(pt2end))
	{
		if(pEdgeFirst->enttype == AcGe::kLine2d)
		{
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			//vtx2d.setBulge(0);
			//vtx2d.setStartWidth(0);
			//vtx2d.setEndWidth(0);
			//vtx2d.setTangent(0);		
			m_vertexArr.push_back(pVertex2d);  //第一个顶点；
			//第二个顶点;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //圆弧；
		{
			//第一个点；
			AcDbArc* pArc = (AcDbArc*)pEdgeFirst->ptrEnt;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(getArcBugle(pArc,true));
			m_vertexArr.push_back(pVertex2d);  
			printVertex(pVertex2d);
			//第二个点。
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kPolyline2d)
		{
			//加入第一个顶点；
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
			//加入第二个顶点；
			AcDbPolyline* pline = (AcDbPolyline*)pEdgeFirst->ptrEnt;
			insertPolyline(pline,false);  //依照ptrEnt原来的顶点顺序，插入新多义线；
		}
		else
		{
			acutPrintf(_T("CToPolyline class::getFirstVertex()002"));
		}
	}
	else
	{
		acutPrintf(_T("ERROR：CToPolyline class::getFirstVertex()003"));
	}
}


//第一个顶点处理完成之后，已经知道第二个顶点position参数。
//此函数求得第二个顶点bugle参数，完善之；然后求得第三个顶点（但无计算bugle参数）
void
CToPolyline::getNextVertex(CEdge* pedge)
{
	AcDb2dVertex* vtx2dLast = m_vertexArr.back(); //上一顶点；
	AcDb2dVertex* vtxNow;
	AcGePoint3d ptLast(vtx2dLast->position());

	//判断起点；
	//pedge的起点为连接点，决定是否要对该弧段反向。
	if(ptLast.isEqualTo(pedge->ptstart))
	{
		if(pedge->enttype == AcGe::kLine2d)
		{
			//第一个vertex不用修改了；
			//添加第二个顶点；
			vtxNow = new AcDb2dVertex();
			vtxNow->setPosition(pedge->ptend);			
			m_vertexArr.push_back(vtxNow);
			printVertex(vtxNow);
		}
		else if(pedge->enttype == AcGe::kCircArc2d)
		{
			//第一个vertex需要修改bugle；
			double fbugle = getArcBugle((AcDbArc*)pedge->ptrEnt,true);
			vtx2dLast->setBulge(fbugle);
			m_vertexArr.pop_back();  //弹出；
			m_vertexArr.push_back(vtx2dLast);  //再加入；
			//添加第二个顶点；
			vtxNow = new AcDb2dVertex();
			vtxNow->setBulge(0);
			vtxNow->setPosition(pedge->ptend);
			m_vertexArr.push_back(vtxNow);
			printVertex(vtxNow);
		}
		else if(pedge->enttype == AcGe::kPolyline2d)
		{
			insertPolyline((AcDbPolyline*)pedge->ptrEnt,false); //按多义线原有顺序插入。
		}
		else
		{
			acutPrintf(_T("ERROR：CToPolyline class::getNextVertex()003"));
		}
	}
	else if(ptLast.isEqualTo(pedge->ptend))
	{
		if(pedge->enttype == AcGe::kLine2d)
		{
			//第一个vertex不用修改了；
			//添加第二个顶点；
			vtxNow = new AcDb2dVertex();
			vtxNow->setPosition(pedge->ptstart);
			m_vertexArr.push_back(vtxNow);
			printVertex(vtxNow);
		}
		else if(pedge->enttype == AcGe::kCircArc2d)
		{
			//第一个vertex需要修改bugle；
			double fbugle = getArcBugle((AcDbArc*)pedge->ptrEnt,false);
			vtx2dLast->setBulge(fbugle);
			m_vertexArr.pop_back();  //先弹出；
			m_vertexArr.push_back(vtx2dLast);  //再加入；
			//添加第二个顶点；
			vtxNow = new AcDb2dVertex();
			vtxNow->setBulge(0);
			vtxNow->setPosition(pedge->ptstart);
			m_vertexArr.push_back(vtxNow);
			printVertex(vtxNow);
		}
		else if(pedge->enttype == AcGe::kPolyline2d)
		{
			insertPolyline((AcDbPolyline*)pedge->ptrEnt,true); //按多义线原有顺序插入。
		}
		else
		{
			acutPrintf(_T("ERROR：CToPolyline class::getNextVertex()003"));
		}
	}
	else
	{
		acutPrintf(_T("ERROR：CToPolyline class::getNextVertex()004"));
	}
}


//计算最后一个顶点的vertex
bool
CToPolyline::getLastVertex()
{
	AcGePoint3d pt3d;
	double startWidth = 0;
	double endWidth = 0;
	double dblBugle = 0;

	int indexLast = getLastVertexIndex();
	if(indexLast >= 0)
	{
		CEdge* pEdge = m_edgeLink->back();
		if(indexLast == pEdge->index1)  //多义线顶点宽度要考虑;
		{
			//计算pt3d，bugle，Startwidth...
			if(pEdge->enttype == AcGe::kPolyline2d)
			{
				((AcDbPolyline*)(pEdge->ptrEnt))->getWidthsAt(0,startWidth,endWidth);		
			}
			pt3d = pEdge->ptstart;
		}
		else
		{
			int nNumVertex = ((AcDbPolyline*)(pEdge->ptrEnt))->numVerts();
			if(pEdge->enttype == AcGe::kPolyline2d)
			{
				((AcDbPolyline*)(pEdge->ptrEnt))->getWidthsAt(nNumVertex-1,startWidth,endWidth);
			}			
			pt3d = pEdge->ptend;
		}
		AcDb2dVertex* pVtx = new AcDb2dVertex;
		pVtx->setPosition(pt3d);
		pVtx->setStartWidth(startWidth);
		pVtx->setEndWidth(endWidth);
		pVtx->setBulge(dblBugle);
		m_vertexArr.push_back(pVtx);		
		return true;
	}
	else
	{
		return false;
	}	
}


//求最后一个顶点AcDbVertex2d；
//并计算新生成的多义线是否闭合；
void
CToPolyline::getLastVertex(CEdge* pedge)
{
	AcDb2dVertex* vtxNow;
	AcDb2dVertex* vtx2dLast = m_vertexArr.back();

	if(pedge->ptstart.isEqualTo(vtx2dLast->position()))
	{
		vtxNow = new AcDb2dVertex();
		vtxNow->setPosition(pedge->ptend);
		vtxNow->setBulge(0);
		m_vertexArr.push_back(vtxNow);
	}
	else if(pedge->ptend.isEqualTo(vtx2dLast->position()))
	{
		vtxNow = new AcDb2dVertex();
		vtxNow->setPosition(pedge->ptstart);
		vtxNow->setBulge(0);
		m_vertexArr.push_back(vtxNow);
	}
}


//判断路径是否闭合;
//取首尾顶点序号，判断其度是否为2？
bool
CToPolyline::isPathClosed()
{
	//取第一个和最后一个顶点序号，判断是否相等； 相等代表闭合;
	int nNum = m_edgeLink->size();
	int index1 = getFirstVertexIndex(); 
	int index2 = getLastVertexIndex();
	if(nNum > 1)
	{
		if(-1 == index1)
		{
			return true;  //说明是2条弧组成的，闭合;
		}
		else
		{
			if(index1 == index2)
			{
				return true;  //闭合;
			}
			else
			{
				return false;  //不闭合；
			}
		}
	}
	else
	{
		return false;  //不闭合;
	}
}


//将CEdge系列（路径）转化为多义线顶点系列；
void
CToPolyline::to_vertex2d()
{
	//清空
	m_vertexArr.clear();

	//检测边表是否为空;
	if(m_edgeLink->size() == 0)
	{
		return;
	}

	//取第一个顶点序号；
	getStartVertexIndex();

	vector<CEdge*>::iterator itr = m_edgeLink->begin();
	for(; itr != m_edgeLink->end(); itr++)
	{
		transEdgeToVetex((CEdge*)(*itr));
	}

	//处理最后一个顶点；最后一个顶点要特殊处理;
	//判断是否闭合; 如果不闭合，则加入最后一个顶点的vertex，不需要bugle;
	m_isclosed = isPathClosed();
	if(m_isclosed)
	{
		return;
	}
	else
	{
		getLastVertex();
	}
}



//AcDbArc实体：都是逆时针转动的，不管初始如何画图;
//根据弧实体，计算其成为多义线组成弧段后的bugle；
//isFromStartPoint表示弧段走向是从起点算bugle，还是从终点计算bugle；
double
CToPolyline::getArcBugle(AcDbArc* pArc,bool isFromStartPoint)
{	
	double startAngle = pArc->startAngle();
	double endAngle = pArc->endAngle();

	double bugleAng = 0;
	if(endAngle < startAngle)
	{
		bugleAng = (endAngle - (startAngle - 2*Pi)) / 4.0;		
	}
	else
	{
		bugleAng  = (endAngle - startAngle ) / 4.0;
	}
	
	double fbugle = tan(bugleAng);
	if(!isFromStartPoint)
	{
		fbugle = - fbugle;
	}
	return fbugle;
}


//测试函数：打印顶点；
void
CToPolyline::printVertex(IN AcDb2dVertex*& pt)
{
#ifdef DEBUG_TO_PL_PRINT
	AcGePoint3d pt1;
	double dblBugle;

	pt1 = pt->position();
	dblBugle = pt->bulge();

	acutPrintf(_T("position is %f，%f  bugle is %f\n"),pt1.x,pt1.y,dblBugle);
#endif
}


//测试函数：打印边表;
void
CToPolyline::printEdgeLink()
{
#ifdef DEBUG_TO_PL_PRINT
	if(m_edgeLink->size() == 0)
	{
		return;
	}

	vector<CEdge*>::const_iterator itr = m_edgeLink->begin();
	acutPrintf(_T("\n此路径信息如下:\n"));
	for(; itr != m_edgeLink->end(); itr++)
	{
		//检查是否有NULL指针
		if((CEdge*)(*itr) == NULL)
		{
			acutPrintf(_T("********************************************************************"));
			acutPrintf(_T("********************************************************************"));
			acutPrintf(_T("********************************************************************"));
			continue;
		}
		//acutPrintf(_T("\n顶点序号(%d,%d)"),((CEdge*)(*itr))->index1,((CEdge*)(*itr))->index2);
		//acutPrintf(_T("顶点坐标(%f,%f)"),((CEdge*)(*itr))->ptstart.x,((CEdge*)(*itr))->ptstart.y);
		//acutPrintf(_T("顶点坐标(%f,%f)\n"),((CEdge*)(*itr))->ptend.x,((CEdge*)(*itr))->ptend.y);
		acutPrintf(_T("\n顶点序号(%d,%d)"),(*itr)->index1,(*itr)->index2);
		acutPrintf(_T("顶点坐标(%f,%f)"),(*itr)->ptstart.x,(*itr)->ptstart.y);
		acutPrintf(_T("顶点坐标(%f,%f)\n"),(*itr)->ptend.x,(*itr)->ptend.y);
	}
	m_edgeLink;

#endif  //#ifdef DEBUG_TO_PL_PRINT
}




//若某条边CEdge是多义线，则需将多义线顶点逐一加入新的多义线。
void
CToPolyline::insertPolyline(IN AcDbPolyline* pLine,bool isReversed)
{
	int numVtx = pLine->numVerts();  //顶点个数;
	AcGeLineSeg2d lnSeg2d;
	AcGeCircArc2d arcSeg2d;
	double fbulge = 0;
	double dblStartWidth = 0;
	double dblEndWidth = 0;
	AcDb2dVertex* vtxNow;
	AcGePoint3d pt3da;

	//if(isReversed)
	//{
	//	pLine->upgradeOpen();
	//	pLine->reverseCurve();  // 将多义线颠倒过来。 //为啥总是报错：notopenforwirte？？！！
	//	pLine->downgradeOpen();
	//}		
	if(!isReversed)  //多义线需要颠倒;
	{
		for(int i = numVtx-1 ; i > 0; i--)  //最后一个顶点这里不需要加入,另外处理;
		{			
			//注意，数据都是颠倒过来的.
			vtxNow = new AcDb2dVertex();
			pLine->getPointAt(i,pt3da);
			vtxNow->setPosition(pt3da);
			pLine->getBulgeAt(i-1,fbulge);
			vtxNow->setBulge(-fbulge);
			pLine->getWidthsAt(i,dblStartWidth,dblEndWidth);
			vtxNow->setStartWidth(dblEndWidth);
			pLine->getWidthsAt(i-1,dblStartWidth,dblEndWidth);
			vtxNow->setEndWidth(dblStartWidth);
			m_vertexArr.push_back(vtxNow);		
		}
	}
	else
	{
		for(int i = 0 ; i < numVtx - 1; i++)  //最后一个顶点这里不需要加入
		{
			/*if(pLine->segType(i) == AcDbPolyline::kLine || pLine->segType(i) == AcDbPolyline::kArc)
			{*/
				vtxNow = new AcDb2dVertex();
				pLine->getPointAt(i,pt3da);
				vtxNow->setPosition(pt3da);
				pLine->getBulgeAt(i,fbulge);
				vtxNow->setBulge(fbulge);
				pLine->getWidthsAt(i,dblStartWidth,dblEndWidth);
				vtxNow->setStartWidth(dblStartWidth);
				vtxNow->setEndWidth(dblEndWidth);
				m_vertexArr.push_back(vtxNow);
			/*}
			else
			{
				acutPrintf(_T("ERROR：CToPolyline::insertPolyline()跳过--001"));
				continue;
			}*/
		}
	}	
}


//将生成的顶点AcDb2dVertex系列做成多义线；
void
CToPolyline::to_polyline(AcDbPolyline*& pLine)
{
	//对CEdge系列加工成顶点系列；
	to_vertex2d();

	AcGePoint2d pt;
	double fbugle = 0;
	double fStartWidth = 0;
	double fEndWidth  = 0;

	vector<AcDb2dVertex*>::iterator itr = m_vertexArr.begin(); 
	for(int i = 0; i < m_vertexArr.size(); i++)
	{
		pt.set((*itr)->position().x,(*itr)->position().y);
		fbugle = (*itr)->bulge();
		pLine->addVertexAt(i,pt,fbugle,fStartWidth,fEndWidth);
		printVertex((AcDb2dVertex*)(*itr));
		itr++;
	}

	//判断多义线是否闭合;	
	if(m_isclosed)
	{
		pLine->setClosed(Adesk::kTrue);  //闭合之。
	}
}

//=============CToPolyline类========================end================;








//=============图类========================================start;

//构造函数，建立图；用图存储所有边实体；
CGraphEnts::CGraphEnts()
{
	numEdges = 0;
	numVertexs = 0;
	m_ssLength = -1;
	m_nDotNum = -1;
}


//析构函数
CGraphEnts::~CGraphEnts()
{
	//释放边占用的内存；
	vector<CEdge* > ::iterator itrEdges = m_vctEdges.begin();
	for(; itrEdges < m_vctEdges.end(); itrEdges++)
	{
		CEdge* pedge = (CEdge*)(*itrEdges);
		if(pedge != NULL)
		{
			pedge->ptrEnt->close();  //关闭打开的实体;
			delete pedge;
		}
	}

	//释放顶点占用的内存；
	vector<CVertex*>::iterator itrVtx = m_vertexTable.begin();
	for(; itrVtx != m_vertexTable.end(); itrVtx++)
	{
		CVertex* pVtx;
		pVtx = (CVertex*)(*itrVtx);
		if(pVtx != NULL)
		{
			delete pVtx;
		}
	}

	//释放visited；
	//delete[] visited;
}



//建立一个边CEdge对象；对象中顶点序号为空，因为还未赋值；
//被insertEdge()调用；
bool 
CGraphEnts::createEdge(CEdge*& pedge,ads_name ssunit)
{
	//pedge = new CEdge();  //新建一条边；	谁用谁创建；

	AcDbObjectId id;
	acdbGetObjectId(id,ssunit);
	AcDbEntity* pent;
	acdbOpenObject(pent,id,AcDb::kForRead);

	AcGe::EntityId enttype;	
	if(pent->isA() == AcDbLine::desc())
	{
		AcDbLine *plineseg = (AcDbLine*)pent;
		pedge->enttype = AcGe::kLine2d;  //线段类型；
		//pedge->m_numSameEdges = 0;
		//pedge->path1 = NULL;
		//pedge->path2 = NULL;
		//pedge->index1 = -1;   // what is index1?
		//pedge->index2 = -1;  // what is index2? how many?
		pedge->ptend.set(plineseg->endPoint().x,plineseg->endPoint().y,plineseg->endPoint().z);
		pedge->ptstart.set(plineseg->startPoint().x,plineseg->startPoint().y,plineseg->startPoint().z);
	}
	else if(pent->isA() == AcDbArc::desc())
	{
		AcDbArc *pArcseg = (AcDbArc*)pent;
		AcGePoint3d pt3d;
		pedge->enttype = AcGe::kCircArc2d;  //线段类型；
		//pedge->m_numSameEdges = 0;
		//pedge->path1 = NULL;
		//pedge->path2 = NULL;
		//pedge->index1 = -1;  //找到顶点序号没有？
		//pedge->index2 = -1;   // 找到顶点序号2没有？
		pArcseg->getEndPoint(pt3d);
		pedge->ptend.set(pt3d.x,pt3d.y,pt3d.z);
		pArcseg->getStartPoint(pt3d);
		pedge->ptstart.set(pt3d.x,pt3d.y,pt3d.z);
		//pedge->ptrSameEdges = NULL;
	}
	else if(pent->isA() == AcDbPolyline::desc())
	{
		AcDbPolyline *pline = (AcDbPolyline*)pent;
		if(pline->isClosed() == kTrue) //闭合多义线不参与运算.
		{
			pent->close();
#ifdef DEBUG_TO_PL_PRINT
			acutPrintf(_T("\n闭合多义线！CGraphEnts.createEdge"));
#endif
			return false;
		}

		AcGePoint3d pt3d;
		pedge->enttype = AcGe::kPolyline2d;  //线段类型；
		//pedge->m_numSameEdges = 0;
		//pedge->path1 = NULL;
		//pedge->path2 = NULL;
		//pedge->index1 = -1;  //找到顶点序号没有？
		//pedge->index2 = -1;   // 找到顶点序号2没有？
		pline->getEndPoint(pt3d);
		pedge->ptend.set(pt3d.x,pt3d.y,pt3d.z);  
		pline->getStartPoint(pt3d);
		pedge->ptstart.set(pt3d.x,pt3d.y,pt3d.z);   
		//pedge->ptrSameEdges = NULL;
	}
	else
	{
		pent->close();
#ifdef DEBUG_TO_PL_PRINT
		acutPrintf(_T("\n线段类型不对啊！CGraphEnts.createEdge"));
#endif
		
		return false;
	}

	//插入顶点，返回顶点序号;  
	int indexStart = insertVertex(pedge->ptstart);
	int indexEnd = insertVertex(pedge->ptend);
	pedge->index1 = indexStart;
	pedge->index2 = indexEnd;
	pedge->path1 = NULL;
	pedge->path2 = NULL;
	pedge->m_leftSameEdges = 1;  //挂在此条边下的相同顶点边的数量，包括自己.用途：计数;
	pedge->m_numSameEdges = 1;  //挂在此条边下的想通顶点边的数量，包括自己；用途？暂时没用上；
	pedge->ptrSameEdges = NULL;
	pedge->ptrEnt = pent;

	pent->close();	
	return true;
}



//寻找图中某条边，返回边的指针；
CEdge* 
CGraphEnts::findEdge(const  AcGePoint3d& pt1,const AcGePoint3d& pt2)
{
	int index1;
	int index2;

	index1 = this->findVertexNode(pt1);
	index2 = this->findVertexNode(pt2);
	if(index2 == -1 || index1 == -1)
	{
		return NULL;
	}

	CEdge* pedge = this->m_vertexTable[index1]->adj;
	while(pedge != NULL)
	{
		if(pedge->index2 == index2)
		{
			return pedge;
		}
		else
		{
			pedge = pedge->path1;  //沿path1继续；
		}
	}

	return NULL;
}



//查找pedge相同的边（即具有相同的顶点及序号；
//pedge是还没插入图；但是已知其顶点序号及顶点；
CEdge*
CGraphEnts::findEdge(IN const CEdge* pedge)
{
	int index1;
	int index2;

	index1 = pedge->index1;
	index2 = pedge->index2;
	if(index2 == -1 || index1 == -1)
	{
		return NULL;
	}

	CEdge* pedgeTemp = this->m_vertexTable[index1]->adj;
	while(pedgeTemp != NULL)
	{
		if(pedgeTemp->index1 == index1)
		{
			if(pedgeTemp->index2 == index2)
			{
				return pedgeTemp;
			}
			else
			{
				pedgeTemp = pedgeTemp->path1;
			}			
		}
		else if(pedgeTemp->index2 == index1)
		{
			if(pedgeTemp->index1 == index2)
			{
				return pedgeTemp;
			}
			else
			{
				pedgeTemp = pedgeTemp->path2;  //沿path2继续；
			}
		}
		else if(pedgeTemp->index2 == index2)
		{
			if(pedgeTemp->index1 == index1)
			{
				return pedgeTemp;
			}
			else
			{
				pedgeTemp = pedgeTemp->path2;  //沿path2继续；
			}
		}
		else if(pedgeTemp->index1 == index2)
		{
			if(pedgeTemp->index2 == index1)
			{
				return pedgeTemp;
			}
			else
			{
				pedgeTemp = pedgeTemp->path1;  //沿path2继续；
			}
		}
		else
		{
			return NULL;
		}
	}  //while
	return NULL;
}



//统计某个节点的度；
//需要检查节点在图中吗？:这里不检查先；
int 
CGraphEnts::getDegreeOfVertex(IN int index)
{
	CEdge* pEdge;
	int idegree = 0; //统计度数；
	pEdge = m_vertexTable[index]->adj;
	while(pEdge != NULL)
	{
		idegree++;
		pEdge = (index == pEdge->index1)?pEdge->path1:pEdge->path2;
	}
	return idegree;
}


//插入节点；
//返回插入的顶点的序号，或者已存在的顶点序号；
int
CGraphEnts::insertVertex(AcGePoint3d& pt)
{
	int index;
	index = findVertexNode(pt);
	if(index == -1)  //查不到有该顶点存在于顶点表中;
	{
		CVertex* pobjVertex = new CVertex(pt);  //建立对象;
		m_vertexTable.push_back(pobjVertex); //收集;
		
		numVertexs++;
		index = numVertexs-1;  //顶点序号从0开始；
		//加入点的访问标志;
		visited.push_back(0);

		//点坐标及序号加入map;
		m_pointMap.insert(pt,index);

		return index; 
	}
	else  //找到了该点存在于顶点表中。则不插入该点，返回已有序号；
	{
		return index;
	}
}



//测试函数，打印边的信息；
void
CGraphEnts::printEdgeInfo(CEdge* pEdge)
{
#ifdef DEBUG_TO_PL_PRINT
	if(pEdge == NULL)
	{
		acutPrintf(_T("此边CEDGE为空指针，退出！\n"));
		return;
	}
	else
	{
		acutPrintf(_T("\n边顶点序号(%d,%d)"),pEdge->index1,pEdge->index2);
		acutPrintf(_T("起点坐标（%f,%f)"),pEdge->ptstart.x,pEdge->ptstart.y);
		acutPrintf(_T("终点坐标（%f,%f)\n"),pEdge->ptend.x,pEdge->ptend.y);
		return;
	}
#endif   //#ifdef DEBUG_TO_PL_PRINT
}


//插入一条边；
//
void
CGraphEnts::insertEdge(CEdge* pedge)
{
	CEdge* pEdgeFind = NULL;
	pEdgeFind = findEdge(pedge); //寻找相同顶点的边；
	if(pEdgeFind != NULL)  //找到了相同的重叠边；挂在重叠边下;
	{
		#ifdef DEBUG_TO_PL_PRINT
		acutPrintf(_T("\n找到了相同的重叠边；挂在重叠边下:"));
		#endif   //#ifdef DEBUG_TO_PL_PRINT

		CEdge* pedgeTemp = pEdgeFind;
		while(pedgeTemp->ptrSameEdges != NULL)
		{
			pedgeTemp = pedgeTemp->ptrSameEdges;  
		}
		pedgeTemp->ptrSameEdges = pedge; //新的边插入到链表末端;   
		pedge->ptrSameEdges = NULL;

		pEdgeFind->m_leftSameEdges++;
		pEdgeFind->m_numSameEdges++;
	}
	else    //没有重叠边，则需要插入成为新的边；
	{
		#ifdef DEBUG_TO_PL_PRINT
		acutPrintf(_T("\n没有重叠边，则需要插入成为新的边:"));
		#endif  //#ifdef DEBUG_TO_PL_PRINT

		CVertex* pVertexStart = m_vertexTable[pedge->index1];
		CVertex* pVertexEnd = m_vertexTable[pedge->index2];
		CEdge* pEdgeStart = pVertexStart->adj;
		CEdge* pEdgeEnd = pVertexEnd->adj;

		pVertexStart->adj = pedge;
		pVertexEnd->adj = pedge;

		pedge->path1 = pEdgeStart; 
		pedge->path2 = pEdgeEnd;

		//pedge->m_leftSameEdges++;  
		//pedge->m_numSameEdges++;  
		numEdges++;
	}

	//测试信息：打印边的信息；测试完可以删除;
	#ifdef DEBUG_TO_PL_PRINT
	acutPrintf(_T("\n插入一条边:"));
	printEdgeInfo(pedge);
	#endif   //#ifdef DEBUG_TO_PL_PRINT	
}



//图的操作之一：删除一条边
bool
CGraphEnts::delEdge(CEdge* pedge)
{
	int index1 = pedge->index1;
	int index2 = pedge->index2;

	//找到上游指针->index1;	
	CEdge* pEdgeTmp = NULL;
	pEdgeTmp = m_vertexTable[index1]->adj;
	while(pEdgeTmp != NULL)
	{		
		if(pEdgeTmp == pedge)
		{
			break;
		}
		pEdgeTmp = pEdgeTmp->path1;
	}
	//断开index1连接;
	if(pEdgeTmp != NULL)
	{
		pEdgeTmp = pEdgeTmp->path1->path1;
		pEdgeTmp->path1 = NULL;
	}
	else
	{
		return false;
	}


	//找到上游指针->index2;	
	pEdgeTmp = NULL;
	pEdgeTmp = m_vertexTable[index2]->adj;
	while(pEdgeTmp != NULL)
	{		
		if(pEdgeTmp == pedge)
		{
			break;
		}
		pEdgeTmp = pEdgeTmp->path2;
	}
	//断开index1连接;
	if(pEdgeTmp != NULL)
	{
		pEdgeTmp = pEdgeTmp->path2->path2;
		pEdgeTmp->path2 = NULL;
	}
	else
	{
		return false;
	}

	return true;
}



//寻找节点，返回找到的节点序号；若找不到，返回-1；
//设置误差; 
//注意：当图里面还没有顶点的时候，要判断之;
//int 
//CGraphEnts::findVertexNode(const AcGePoint3d& pt)
//{
//	AcGeTol tola;
//	tola.setEqualPoint(0.0001);
//	int index;
//	for(int i = 0; i < this->numVertexs; i++)
//	{
//		if(pt.isEqualTo(this->m_vertexTable[i]->pt,tola))
//		{
//			return i;
//		}
//	}
//	return -1;
//}
int 
CGraphEnts::findVertexNode(const AcGePoint3d& pt)
{
	int ptIndex = -1;
	bool bflag = m_pointMap.find(pt,ptIndex);
	return ptIndex;
}



//获取v1的第一个邻接顶点；返回其顶点序号；
//获取邻接顶点后，顺便获取俩顶点关联的边；
////////////////1.因为这里是多重邻接表，所以要带上父亲节点来查找第一个邻接点；
////////////////2.否则第一个邻接点又会查找到父节点，死循环了！
int
CGraphEnts::GetFirstNeighbor(int v1,OUT CEdge*& pedge)
{
	//CEdge* pedge; 
	if(v1 != -1)
	{
		pedge = this->m_vertexTable[v1]->adj;
		if(pedge != NULL)
		{
			return (v1 == pedge->index1)?pedge->index2:pedge->index1;
		}	
		else
		{
			return -1;
		}
	}
	return -1;
}




// 获取顶点v1的邻接点v2下一个邻接顶点；
int
CGraphEnts::GetNextNeighbor(const int v1,const int v2,CEdge*& pedge)
{
	//int index1;
	//int index2;
	if(v1 == v2)
	{//此处没必要；
		#ifdef DEBUG_TO_PL_PRINT
		acutPrintf(_T("CGraphEnts::GetNextNeighbor(const int v1,const int v2,CEdge*& pedge),v1 == v2 error"));
		#endif  //#ifdef DEBUG_TO_PL_PRINT
		return -1;
	}

	if(v1 != -1)
	{
		pedge = m_vertexTable[v1]->adj;
		while(pedge != NULL)
		{
			//走向问题，下一步怎么走;
			if(v2 == pedge->index1)
			{
				pedge = pedge->path2;
				if(pedge == NULL)
				{
					return -1;
				}
				return (v1 == pedge->index2)?pedge->index1:pedge->index2;
			}
			else if(v2 == pedge->index2)
			{
				pedge = pedge->path1;
				if(pedge == NULL)
				{
					return -1;
				}
				return (v1 == pedge->index2)?pedge->index1:pedge->index2;
			}
			else  //走向下一条边；
			{
				pedge = (v1 == pedge->index1)?(pedge->path1):(pedge->path2);
			}
		}
	}	
	return -1;
}



//重置visited数组；
void
CGraphEnts::resetvisited()
{
	vector<int>::iterator itr = visited.begin();
	for(; itr != visited.end(); itr++)
	{
		(*itr) = 0;
	}
}


//深度遍历图；
//遍历过程中获取回路，提取环制作closed polyline；
//也提取不闭合的路径，制作not closed polyline；
//不采用递归方式；为什么？：因为递归过程中中断不好处理；非递归也挺简单的；
//采用栈结构保存节点，非递归深度遍历；当提取回路后
void
CGraphEnts::DFS(int index)
{
	if(index >= this->numVertexs)  //index不合法；
	{
		return;
	}

	stack<int>  stackVertex;  //遍历辅助用栈；
	stack<CEdge*> stackEdges;  //存放回路边；也是栈；
	CEdge* pedge = NULL;  //指向边的指针；

	visited[index] = 1;  
	stackVertex.push(index);
	m_stackEdges.push_back(index);  //这个是CArcLink;第一个顶点入栈；
	while(!stackVertex.empty())   //循环深度遍历；
	{
		int iparent = stackVertex.top();  //注意，只是取顶部元素；不是弹出；
		int ineighbor = iparent;		//赋初值而已，无其他意图；>=0即可；	
		//GetFirstNeighbor()返回值肯定>=0,不等于-1；GetNextNeighbor则不一定了；	
		ineighbor = GetFirstNeighbor(iparent,pedge); 

		while(ineighbor != -1)
		{
			//GetFirstNeighbor()返回值肯定>=0,不等于-1；GetNextNeighbor则不一定了；
			//ineighbor = GetFirstNeighbor(iparent,pedge); 
			if(visited[ineighbor] == 0)
			{
				//进栈：因为该节点没有被访问过；
				m_stackEdges.push_back(pedge);  //边入栈;

				//访问ineighbor;
				visited[ineighbor] = 1;
				stackVertex.push(ineighbor);  //进栈；
				m_stackEdges.push_back(ineighbor);  //顶点入栈；此步骤可能做重复了^_^

				//取下一个邻接顶点；
				//itemp = ineighbor;   //保存之，随后会赋值给iparent；此处不改变iparent值；  
				//ineighbor = GetFirstNeighbor(ineighbor,pedge);  //更新iparent,ineighbor; 


				//在这里收集回路，或者非回路链路;  
				testIfLoopONextVtxNode(ineighbor);

				iparent = ineighbor; //下一步；
				ineighbor = GetFirstNeighbor(iparent,pedge);
			}
			else
			{
				ineighbor = GetNextNeighbor(iparent,ineighbor,pedge);	
			}
		} //while;
		stackVertex.pop(); //弹出？:因为此节点的邻接点都遍历完了，所以弹出它；
		m_stackEdges.pop_edge();
		m_stackEdges.pop_index();  //弹出顶点；
	} //while
}


//图的所有顶点都被拆分过了，每个顶点只有1-2度；所以直接提取一条边表;
//
void CGraphEnts::DFS2(int iIndex)
{
	//判断顶点是否被访问过；
	if(iIndex < 0 || iIndex >= numVertexs || visited[iIndex] == 1)
	{
		#ifdef DEBUG_TO_PL_PRINT
		acutPrintf(_T("\n DFS2():numVertexs is %d ."),numVertexs);
		acutPrintf(_T("DFS2():vertex is %d "),iIndex);
		#endif
		return;
	}

	//检查该顶点的度;
	int i = getDegreeOfVertex(iIndex);
	if(i > 2)
	{
		acutPrintf(_T("\n顶点%d,度为%d 不等于1跳过..."),iIndex,i);
		return;
	}

	//遍历度为1的顶点；
	DFS(iIndex);
}


//提取边链表; 
//前提是：图中所有顶点的度小于等于2;
void
CGraphEnts::extractEdgeLinks(IN const int iIndex)  
{
	//check if it is visited
	if(visited[iIndex] == 1)
	{
		return;
	}
	visited[iIndex] = 1; //set;

	//init;	
	m_stackEdges.clear();

	//check iIndex
	if(!isVertexIndexValid(iIndex))
	{
		return;
	}

	//获取第一段;
	vector<CEdge*> vEdgeLinksFirst;  //收集第一段边链表;
	vector<CEdge*> vEdgeLinksSecond; //收集第二段边链表;
	int iVtxIndex = iIndex; 
	int iParent = iIndex;
	int iNewVtx = -1;  //存放新获取的顶点序号;
	CEdge* pEdge = NULL;
	iVtxIndex = GetFirstNeighbor(iParent,pEdge);
	int iBak = iVtxIndex;
	while(isVertexIndexValid(iVtxIndex)) //合法顶点序号;
	{
		visited[iVtxIndex] = 1;
		vEdgeLinksFirst.push_back(pEdge);
		iNewVtx = getAnotherVertex(iParent,iVtxIndex,pEdge);
		if(isVertexIndexValid(iNewVtx))
		{
			if(visited[iNewVtx] == 1)  //被访问过了;
			{
				vEdgeLinksFirst.push_back(pEdge);
				break;
			}
		}
		//交换顺序;
		iParent = iVtxIndex;
		iVtxIndex = iNewVtx;
	}

	//获取第二段;
	iVtxIndex = -1; 
	iParent = iIndex;
	iNewVtx = -1;  //存放新获取的顶点序号;
	pEdge = NULL;
	iVtxIndex = GetNextNeighbor(iIndex,iBak,pEdge);
	if(isVertexIndexValid(iVtxIndex))
	{
		if(visited[iVtxIndex] == 0)
		{
			while(isVertexIndexValid(iVtxIndex)) //合法顶点序号
			{
				visited[iVtxIndex] = 1;
				vEdgeLinksSecond.push_back(pEdge);
				iNewVtx = getAnotherVertex(iParent,iVtxIndex,pEdge);
				//交换顺序;		
				iParent = iVtxIndex;
				iVtxIndex = iNewVtx;
			}
		}	
	}	

	//合并两段;
	//合并第一段;
	CEdge* pEdgeTmp = NULL;
	std::reverse(vEdgeLinksFirst.begin(),vEdgeLinksFirst.end()); //倒立;
	vector<CEdge*>::iterator itr = vEdgeLinksFirst.begin();
	for(; itr != vEdgeLinksFirst.end(); itr++)
	{
		pEdgeTmp = (CEdge*)(*itr);
		m_stackEdges.push_back(pEdgeTmp);
	}
	//合并第二段;
	pEdgeTmp = NULL;
	itr = vEdgeLinksSecond.begin();
	for(; itr != vEdgeLinksSecond.end(); itr++)
	{
		pEdgeTmp = (CEdge*)(*itr);
		m_stackEdges.push_back(pEdgeTmp);
	}

	m_stackEdges.testPintOriginalPath();
	m_allLoops.push_back(m_stackEdges);
}


//深度遍历，打印图的边信息;
//递归;
//若递归深度太多，则会导致cad当机；目前递归到12680会挂掉cad; 改用广度遍历吧.
void CGraphEnts::testDFS3(IN int iIndex)
{
	visited[iIndex] = 1;  //标记成已经访问过;
	CEdge* pEdge = NULL;

	int iNextVtxIndex = GetFirstNeighbor(iIndex,pEdge);
	if(visited[iNextVtxIndex] == 0)
	{
		//打印边信息;
		printEdgeInfo(pEdge);
		testDFS3(iNextVtxIndex);
	}

	//搜索下一条邻接边;
	do
	{
		iNextVtxIndex = GetNextNeighbor(iIndex,iNextVtxIndex,pEdge);
		if(iNextVtxIndex != -1)
		{
			if(visited[iNextVtxIndex] == 0)
			{
				//打印边信息;
				printEdgeInfo(pEdge);
				testDFS3(iNextVtxIndex);
			}
		}		
	}while(iNextVtxIndex != -1);
}


//测试用，打印图的所有边信息;
void 
CGraphEnts::DFS3()
{
	resetvisited();  //所有顶点全部设置为未访问状态;

	//acutPrintf(_T("\nDFS3():--start"));
	for(int i = 0; i < numVertexs; i++)
	{
		if(visited[i] == 0)
		{
			testDFS3(i);
		}
	}
	//acutPrintf(_T("\nDFS3():--end"));
}


//取某顶点iParent的除v1外的另一顶点，前提是该顶点度为2;
//若找不到，返回-1;
int
CGraphEnts::getAnotherVertex(IN const int v1,IN const int iParent,IN CEdge*& pEdge)
{
	if(!isVertexIndexValid(v1) || !isVertexIndexValid(iParent))
	{
		return -1;
	}

	int iDegree = 0;
	iDegree = getDegreeOfVertex(iParent);
	if(iDegree != 2)
	{
		return -1;
	}

	//取第一个邻接点;
	int iIndexFirst = -1;
	int iIndexSecond = -1;
	CEdge* pEdgeFirst = NULL;
	CEdge* pEdgeSecond = NULL;
	iIndexFirst = GetFirstNeighbor(iParent,pEdgeFirst);
	iIndexSecond = GetNextNeighbor(iParent,iIndexFirst,pEdgeSecond);	
	if(iIndexFirst == v1)
	{
		pEdge = pEdgeSecond;
		return iIndexSecond;
	}
	else if(iIndexSecond == v1)
	{
		pEdge = pEdgeFirst;
		return iIndexFirst;
	}
	else
	{
		pEdge = NULL;
		return -1;
	}
}


//测试顶点序号是否合法;
bool
CGraphEnts::isVertexIndexValid(IN const int iVertexIndex)
{
	if(iVertexIndex < 0 || iVertexIndex > numVertexs)
	{
		return false;
	}
	return true;
}


//对每条边提取起重叠边构成的环（也是提取环路）
//直接按顶点集合来遍历?
bool
CGraphEnts::extractOverlappedEdges() 
{
	int iIdx = 0;   //图顶点序号
	CEdge* pEdge = NULL;  //指向边的临时指针.
	//循环所有顶点
	for(; iIdx < numVertexs; iIdx++)
	{
		//循环某个顶点下的所有边;
		pEdge = m_vertexTable[iIdx]->adj;
		while(pEdge != NULL)
		{
			extractOverlappedEdge(pEdge);
			if(pEdge->m_leftSameEdges == 0) //说明全部被提取了
			{
				delEdge(pEdge); //删除之后，
			}
			//指针往下移动;
			if(iIdx == pEdge->index1)
			{
				pEdge = pEdge->path1;
			}
			else
			{
				pEdge = pEdge->path2;
			}			
		}
	}
	return true;
}



//对指定边提取重叠边构成的环路（loop links);
//提取的环路存放于CArcLinkArray; 不用对其初始化;
bool
CGraphEnts::extractOverlappedEdge(IN CEdge* pEdge)
{
	//CEdge* pEdge;
	int edgeDepth = 0;	
	CEdge* pEdgeTemp = pEdge;
	edgeDepth = pEdge->m_leftSameEdges;
	if(edgeDepth > 1)  //此边重叠数超过1，则需将重叠边每两个一组取出放于pEdgeLinks；
	{
		int i = 0;
		i = edgeDepth % 2;   //测试奇偶;
		if(1 == i)
		{
			pEdgeTemp = pEdgeTemp->ptrSameEdges;
		}
		while(pEdgeTemp)
		{
			m_stackEdges.push_back(pEdgeTemp);
			pEdgeTemp = pEdgeTemp->ptrSameEdges;
			m_stackEdges.push_back(pEdgeTemp);
			pEdgeTemp = pEdgeTemp->ptrSameEdges;  //bug  

			m_allLoops.push_back(m_stackEdges); 
			m_stackEdges.clear();
			pEdge->m_leftSameEdges -= 2;   //此边剩余数量每次减去2，最后可能是0或者1；
		}
		return true;
	}
	return false;
}



//分析点坐标小数位数量、获取误差值;
//通过ads_name实体集合，判断图形最外框大小，初略确定点坐标精度：
//1.点坐标小数位; 2.点坐标相等误差（pointEqual）-实际用不上；
void 
CGraphEnts::prsDotNumAndTol()
{
	CGetPointEqualAcGeTol objTol;
	AcGeTol equalPointTol;
	equalPointTol = objTol.getPointTolerance(m_sel,m_nDotNum);
}


//根据顶点序号，获取其全部邻接顶点，判断是否形成路径；
//形成路径标准：1、邻接点为叶子； 2、邻接点构成环（被访问过）；
//如果形成路径，加入；
void
CGraphEnts::testIfLoopONextVtxNode(const int v1) //,CArcLink& objCArcLink)
{
	CEdge* pEdge = NULL;
	pEdge = m_stackEdges.back(); //取尾部元素，不弹出；

	CEdge *pEdgeFind = NULL;
	int v2 = GetFirstNeighbor(v1,pEdgeFind);
	while(v2 != -1)
	{
		if(visited[v2] == 0) //没有被访问过；
		{
			if(getDegreeOfVertex(v2) == 1) //叶子；加入；
			{
				m_stackEdges.push_back(pEdgeFind);
				m_allLoops.push_back(m_stackEdges);
				//加完了之后,退一下；
				m_stackEdges.pop_edge();
			}
		}
		else  //访问过的，判断是否环路；
		{
			if(pEdgeFind != pEdge)
			{
				m_stackEdges.push_back(pEdgeFind);
				m_allLoops.push_back(m_stackEdges);
				//加完了之后,退一下；
				m_stackEdges.pop_edge();
			}
		}
		v2 = GetNextNeighbor(v1,v2,pEdgeFind);  //遍历所有邻接点。
	}
}




//造图；
//请用户选择图形，然后造图；
void
CGraphEnts::createGraph()
{
	long numOfss;
	acedSSLength(m_sel,&numOfss);
	for(long i = 0; i < numOfss; i++)
	{
		ads_name sstmp;
		acedSSName(m_sel,i,sstmp);
		CEdge* pedge = new CEdge;
		if(!createEdge(pedge,sstmp))
		{
			delete pedge;
			continue;
		}
		else
		{
			m_vctEdges.push_back(pedge);  //将边收集起来;
			insertEdge(pedge);
		}
	}
}


//遍历所有顶点;
void
CGraphEnts::chopEdgeLinks()
{
	//检查顶点数
	//if(m_vertexTable.size() <= 0)
	//{
	//	return;
	//}
	for(int i = 0; i < numVertexs; i++)
	{
		chopVertex(i);
	}
}


//对某个顶点进行判断，若度数大于2，则拆分;
//每个顶点只允许度数为2;
//拆分方法：
//1.没隔2条边拆分一次；
//2.每拆分一次，剩余边链表必须即时更新顶点，每条边的重叠边也要即时更新;
void 
CGraphEnts::chopVertex(IN const int iIndex)
{
	int idxEdge = iIndex;
	int nCal = 0; //计算用，当为偶数时分离出一个相同顶点;   
	CEdge* ptrNext = m_vertexTable[iIndex]->adj; //第一条边;  
	CEdge* ptrEdge = NULL;
	while(ptrNext != NULL)
	{
		for(int i = 0; ptrNext != NULL && i < 2; i++)  //一次遍历2条边;  
		{
			if(idxEdge == ptrNext->index1)
			{
				ptrEdge = ptrNext; //指向边;
				ptrNext = ptrNext->path1;
			}
			else
			{
				ptrEdge = ptrNext;
				ptrNext = ptrNext->path2;  
			}
		}

		if(ptrNext == NULL) //check
		{
			break;
		}
		else  //边指针指向null;
		{
			if(idxEdge == ptrEdge->index1)
			{
				ptrEdge->path1 = NULL; //指向边;
			}
			else
			{
				ptrEdge->path2 = NULL;
			}

			//ptrEdge = NULL;
		}

		//插入新顶点; 拆分原来顶点;
		int iNewIndex = insertSameVertex(iIndex);
		m_vertexTable[iNewIndex]->adj = ptrNext;
		//把该顶点链表下所有顶点序号都更新成新顶点;
		CEdge* pEdge = ptrNext;
		while(pEdge != NULL)
		{
			//对后续每条边的重叠边的序号也要更新;
			//这里不能更改pEdgeTmp,留下一步改;
			CEdge* pEdgeTmp = pEdge->ptrSameEdges; 
			while(pEdgeTmp != NULL)
			{
				if(idxEdge == pEdgeTmp->index1)
				{
					pEdgeTmp->index1 = iNewIndex;
				}
				else if(idxEdge == pEdgeTmp->index2)
				{
					pEdgeTmp->index2 = iNewIndex;
				}
				pEdgeTmp = pEdgeTmp->ptrSameEdges;
			}

			//后续连续边的相应顶点序号更新;
			if(idxEdge == pEdge->index1)
			{
				pEdge->index1 = iNewIndex;
				pEdge = pEdge->path1;
			}
			else
			{
				pEdge->index2 = iNewIndex;
				pEdge = pEdge->path2;
			}
		}
		idxEdge = iNewIndex;  //更新边序号;	
	}
}



//根据当前顶点生成新顶点（肯定是重复顶点）
//返回值：顶点序号;
int
CGraphEnts::insertSameVertex(IN const int vtxIndex)
{
	//int index;
	//index = findVertexNode(pt);
	AcGePoint3d pt;
	pt = m_vertexTable[vtxIndex]->pt;
	if(vtxIndex >= 0)  //顶点合法；
	{
		CVertex* pobjVertex = new CVertex(pt);  //建立对象；
		m_vertexTable.push_back(pobjVertex);
		visited.push_back(0); //随之插入访问标记;

		numVertexs++;
		int index = numVertexs -1;  //序号从0开始；	
		return index; 
	}
	else  //找到了该点存在于顶点表中。则不插入该点，返回-1；一般不会出现此种情况;
	{
		return -1;
	}
}


//判断2个点是否相等;
//错误，需修改;
bool 
CGraphEnts::isTwoPointsEqual(IN const AcGePoint3d& pt1,IN const AcGePoint3d& pt2,IN const int nDotNUm)
{
	double x1 =  CPointMap::transByDotNum(pt1.x,nDotNUm);
	double y1 =  CPointMap::transByDotNum(pt1.y,nDotNUm);
	double x2 =  CPointMap::transByDotNum(pt2.x,nDotNUm);
	double y2 =  CPointMap::transByDotNum(pt2.y,nDotNUm);
	if(x1 == x2 && y1 == y2)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//深度遍历图后，得到路径集合；
//然后，对路径对照真实的边实体，整理出真实路径（因为遍历路径之间可能重复使用某些边，
//或者真实路径是有重叠）；
void 
CGraphEnts::to_polyline(IN ads_name ssUsr) 
{
	CTimeElapse objTimeElpsed; //统计时间; 	

	ads_name_set(ssUsr,m_sel);  //选择集  

	//分析点坐标相等误差值，小数位数;
	if(ads_name_nil(ssUsr))
	{
		acutPrintf(_T("\n user select pickset is empty."));
		return;	
	}
	else
	{
		prsDotNumAndTol();
	}

	createGraph();  //建图;

	CTimeElapse objTimeElpsedAnother;

	//提取重叠边
	extractOverlappedEdges();	

	//DFS3(); //打印图；测试语句

	chopEdgeLinks(); //(出）入度大于3的顶点拆分之，使得出入度小于3；

	//DFS3();  //打印图，测试语句;

	//从0开始遍历；提取所有路径;
	resetvisited(); //先设置一下访问标识;
	for(int i = 0; i < numVertexs; i++)
	{
		extractEdgeLinks(i);
	}
	//测试：打印遍历取得的路径;
	

	//打印获取的路径；测试完了后删除;
	m_allLoops.testPrintOriginalPath();

	m_allLoops.correctByRealEdges();

	////输出用来生成多义线的路径（已经调整好的路径）;
	m_allLoops.testm_edgeLinkArrayToPolyline();

	m_allLoops.toPolylines();
}

//=============以上为图类========================================end======




//==========================
//class  CGetAcGeTol
//输入：图形集合
//输出：；
//==========================
CGetPointEqualAcGeTol::CGetPointEqualAcGeTol()
{
	m_tol = 0.000001;  //默认值;
}

CGetPointEqualAcGeTol::~CGetPointEqualAcGeTol()
{
}


//计算图形实体集合的最外框线及坐标值；根据坐标值的范围，适当调整
//误差值计算规则：
//1.0默认是0.000001；
//1.1如果最大坐标值超过1000，设置为0.00001;
//1.2如果最大坐标值小于10，设置为0.0000001; 以上。
AcGeTol 
CGetPointEqualAcGeTol::getPointTolerance(IN ads_name ss,OUT int& nDotNum)
{
	const double dbltol = 0.000001;  //default;
	AcGeTol objEqualPoint;
	objEqualPoint.setEqualPoint(dbltol);
	nDotNum = 6;

	//objEqualPoint.setEqualPoint(m_tol);

	long lLen;
	acedSSLength(ss,&lLen);
	if(lLen == 0)
	{
		return objEqualPoint;
	}

	CExtentOfSS objExt;
	objExt.getExtents(ss);
	//double xlen;
	//double ylen;
	AcGePoint3d ptLeftBot;
	AcGePoint3d ptRightTop;
	//objExt.getLenX(xlen);
	//objExt.getLenY(ylen);
	objExt.getMaxPoint(ptRightTop);
	objExt.getMinPoint(ptLeftBot);
	//判断点坐标数值级别;
	double xRT = ptRightTop.x;
	double yRT = ptRightTop.y;
	double xLB = ptLeftBot.x;
	double yLB = ptLeftBot.y;

	xRT = abs(xRT);
	yRT = abs(yRT);
	xLB = abs(xLB);
	yLB = abs(yLB);

	//按规则;
	if(xRT >= 1000 || yRT >= 1000 || xLB > 1000 || yLB > 1000)
	{
		objEqualPoint.setEqualPoint(0.00001);
		nDotNum = 5;
		return objEqualPoint;
	}
	else if(xRT <=  10 || yRT <=  10 || xLB <=  10 || yLB <=  10)
	{
		objEqualPoint.setEqualPoint(0.0000001);
		nDotNum = 7;
		return objEqualPoint;
	}
	else
	{
		return objEqualPoint;
	}
}


//=====================================
//命令topln
//=====================================
void 
to_polylines(void)
{
	//用户选择实体集合;
	ads_name ssUsr;
	CSelectEnts objSelect;
	objSelect.usrSelect(ssUsr);

	CGraphEnts obj;
	obj.to_polyline(ssUsr); 

	//release pickset
	acedSSFree(ssUsr);   
	// Add your code for command ZHGpc2010.owal2016topolyline here
}