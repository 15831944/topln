/************************************************************
Copyright (C), 2016-6-12, shiyuan procision maskmaking. Co., Ltd.
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
#include "..\StdAfx.h"
#include  "to_pl.h"
#include "selectss.h"
#include "TCHAR.h"
//=============CEdge��========================================start=======
//���캯��
CEdge::CEdge()
{
	index1 = -1;    //����1��ͼ�е���ţ�
	index2 = -1;     //����2��ͼ�е���ţ�
	path1 = NULL;  //��һ���������һ���ڽӱߣ�
	path2 = NULL;   //�ڶ����������һ���ڽӱߣ�
	//AcGePoint3d ptstart;  //�߶���㣻
	//AcGePoint3d ptend;   //�߶��յ㣻
	ptrSameEdges = NULL;  //ͬһ���߿����ж��ʵ��ͼԪ��line��arc��polyline����
	ptrEnt = NULL;
	//AcGe::EntityId enttype;   //����ʵ����ʲô�ࣺline��or arc��polyline������
	//AcDbEntity* pEnt;   //ָ��һ��ʵ�壻ָ��Ƚ�ʡ�ռ䡣

	int m_numSameEdges = 0;   //�˱ߣ�ͼ�ṹ�����м���ʵ��ͼԪ�ߣ��ص��ģ� 
	m_leftSameEdges = 0;  //ͳ�ƴ����߱�����֮�󣬻�ʣ��������Ϊ0�����������ˣ������õ��������������ȡ��·��
}

//��������;
CEdge::~CEdge()
{
}
//=============CEdge��========================================end=======



//=============CVertex��========================================start=======
//���캯��
CVertex::CVertex()
{
}
//��������
CVertex::~CVertex()
{
}

//��������2
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
//=============CVertex��========================================end=======





//=============CArcLink��========================================start=======
//�߼����ࣻ
//���캯����
CArcLink::CArcLink()
{
	isClosed = false;
}

//����������
CArcLink::~CArcLink()
{
}



//�жϴ˱������Ƿ�պϣ�
//ǰ�᣺��·������������������˵ģ�;
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
	int index2_two;//��ʱ����
	int index_last; //���һ���ߵĽ�β���㣻
	int index_first; //��һ���ߵ���ʼ���㣻

	vector<CEdge*>::iterator itr = m_edgesLink.end();
	itr--; //����һ�£�����ָ���β�ڵ㣻�����ǽ�����
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	//��·��ֻ��2����ʱ
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

	//��ȡ�����һ���ߵĽ�β������ţ�
	//ȷ��β��β���㣻
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
	//�����һ���������ʼ�㣻
	itr = m_edgesLink.begin();
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	itr++;
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	if(index1_one == index1_two || index1_one == index2_two)
	{
		index_first = index2_one;  //�õ�ͷ�����š�
	}
	else if(index2_one == index1_two || index2_one == index2_two)
	{
		index_first = index1_one;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//�ж�ͷ����Ƿ��β�ڵ���ͬ��
	CArcLink* pArcLink ;
	if(index_first != index_last)
	{
		isClosed= false;
		return isClosed;
	}
	else
	{
		isClosed = true;
		return isClosed;  //����ѭ����.�˳���������
	}
}




//���ã�·���Ƿ�պϲ���
void
CArcLink::setClosed(bool isitclosed)
{
	isClosed  = isitclosed;
}



//���CEdge����
void
CArcLink::push_back(CEdge* pedge)
{
	if(pedge !=  NULL)
	{
		m_edgesLink.push_back(pedge);
	}
	else
	{
		acutPrintf(_T("\nCArcLink::push_back(CEdge* pedge):pedgeΪ��-error"));
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
		acutPrintf(_T("\nCArcLink::push_back(CEdge* pedge):pedgeΪ��-error"));
	}
}


//ȡβ��Ԫ�أ�
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



//����β��CEdge����
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


//��Ӷ���˳��ţ�
void
CArcLink::push_back(int index)
{
	if (index != -1)
	{
		m_vectroIndex.push_back(index);
	}
}


//ȡedge������
int
CArcLink::sizeOfEdges()
{
	return m_edgesLink.size();
}


//���
void
CArcLink::clear()
{
	m_edgesLink.clear();
}


//����β���ڵ����;
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
	return -1;  //����-1��ʾ����
}




//�Ապ�·����������ȥ���ǻ����ֵıߣ���Ϊ��ͼ����ȡ����ʱ�򣬻����ܴ���β�ͣ�
//β��ȥ���󣬽����µ�CArcLink,���¼���CArcLinkArray��
void
CArcLink::correctLinks(CArcLinkArray* pLinkArr)
{
	if(m_edgesLink.size() <= 2  || isClosed == false)  //���·�������Լ��Ƿ�պϣ�
	{
		return;
	}

	int index1_one;
	int index2_one;
	int index1_two;
	int index2_two;//��ʱ����
	int index_last; //���һ���ߵĽ�β���㣻
	int index_first; //��һ���ߵ���ʼ���㣻

	//ÿ���ߺͽ�β�߽�β������Ž��бȽϣ�����Ƿ�պϣ�����ɾ��֮��
	//��ȡ�����һ���ߵĽ�β������ţ�
	vector<CEdge*>::iterator itr = m_edgesLink.end();
	itr--; //�˻�һ��������itr��û������ġ�understand��
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	//ȷ�����Ķ��㣻
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
	//�����һ���������ʼ�㣻
	itr = m_edgesLink.begin();
	index1_one = (*itr)->index1;
	index2_one = (*itr)->index2;
	itr++;
	index1_two = (*itr)->index1;
	index2_two = (*itr)->index2;
	itr--;
	if(index1_one == index1_two || index1_one == index2_two)
	{
		index_first = index2_one;  //�õ�ͷ�����š�
	}
	else if(index2_one == index1_two || index2_one == index2_two)
	{
		index_first = index1_one;
	}
	else
	{
		acutPrintf(_T("CArcLink::correctLinks() error log"));
	}

	//�ж�ͷ����Ƿ��β�ڵ���ͬ��
	CArcLink pArcLink ;
	if(index_first != index_last)
	{
		//pArcLink = new CArcLink();
		pArcLink.push_back((CEdge*)(*itr));
		itr = m_edgesLink.erase(itr); //ɾ����ǰ�ߣ�itrָ����һ���ߣ�
	}
	else
	{
		return;  //����ѭ����.�˳���������
	}

	//����ȥ���ǱպϵĲ��֣�
	for(; itr < m_edgesLink.end(); itr++)
	{
		//�����һ���������ʼ�㣻
		index1_one = (*itr)->index1;
		index2_one = (*itr)->index2;
		itr++;
		index1_two = (*itr)->index1;
		index2_two = (*itr)->index2;
		itr--;
		if(index1_one == index1_two || index1_one == index2_two)
		{
			index_first = index2_one;  //�õ�ͷ�����š�
		}
		else if(index2_one == index1_two || index2_one == index2_two)
		{
			index_first = index1_one;
		}
		else
		{
			acutPrintf(_T("CArcLink::correctLinks() error log"));
		}

		//�ж�ͷ����Ƿ��β�ڵ���ͬ��
		if(index_first != index_last)
		{
			pArcLink.push_back((CEdge*)(*itr));
			itr = m_edgesLink.erase(itr); //ɾ����ǰ�ߣ�itrָ����һ���ߣ�
		}
		else
		{
			break;  //����ѭ����.
		}
	}
	pLinkArr->push_back(pArcLink);
}



//����ʵ�ʵı�ʵ��ͼ��������ĳ��·���Ƿ�Ӧ�ã�1.>=1,���ظ����ɣ�2.==0����ѳɶ����ߣ���ʵ�ģ���
//�˺������ܼ������path������׼����������ʵ�����ɶ����ߵ�path��
//��������ص��ߵ��ص�����
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
	acutPrintf(_T("\n��С�ص�����Ϊ%d"),m_numOfLoops);
}


//��ȡclosed path��ÿ����ȡһ����
//�˺����Ѿ��ٶ��л�������ȡ��
//һ����CEdge��ȡһ�κ������������������
//ÿ����ȡCEDGE���Ǵ��������ڵ���ȡ������ڵ��������ڱ�ģ�����ڵ㲢���䣻
void 
CArcLink::extractLoops(vector<vector<CEdge*>> & pEdgesLinks)
{
	//if(isClosed == false)  //�ȼ���Ƿ�պϣ�//���պ�ҲҪ��ȡ�ص���·2017-11-20;
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
			//���pEdgeʣ��ֵ����ѡ��ӦpEdge;��ΪpEdge���ص�;
			//�ҵ����һ��pedge��
			int i;
			//pEdge = (*itr)->ptrSameEdges;
			pEdge = (CEdge*)(*itr);
			for(i = 1; i < (*itr)->m_leftSameEdges; i++)  //�� 1��ʼ;����ȡ�ص��������һ����;
			{
				pEdge = pEdge->ptrSameEdges; //���룻
			}
			pVectEdges.push_back(pEdge);

			//CEdge����ȡ�����������Ӧ�޸ģ�
			//�޸�ʣ��CEdge��
			(*itr)->m_leftSameEdges--;			
		}
		pEdgesLinks.push_back(pVectEdges);
		pVectEdges.clear();

		m_numOfLoops--;  //���ձ�Ϊ0��
	}
	//���ϡ� Ȼ���������pedge�ص��ı߹��ɵĻ���2�߻�����	
}



//��ȡ����·���ϵĻ�������ȡÿ�����ϵĻ�����Ϊ�߿������ص��ߣ���
//�Դ˱��ص��߳ɶ���ȡ��ÿ�ԣ�2��������һ������
//ע��CEdge�ϵĸ��ֲ����ı䶯��
//�˴��ȼ���ʣ������������ż����Ȼ������Ƿ�������1���ߣ�Ȼ��ɶ���ȡ��
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
		if(edgeDepth > 1)  //�˱��ص�������1�����轫�ص���ÿ����һ��ȡ������pEdgeLinks��
		{
			int i = 0;
			i = edgeDepth % 2;   //������ż;
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
				pEdge->m_leftSameEdges -= 2;   //�˱�ʣ������ÿ�μ�ȥ2����������0����1��
			}
		}
	}
}


//��ȡnot closed path����
//ǰ��:�ص����Ѿ�����ȡ����;
//��ʵÿ����ʣ���ص��߲���0����1��ֻ����ȡһ���ˣ�
//���ݱߵ��ص�������ȡ������m_leftSameEdges��
//
void 
CArcLink::extractPathNotClosed(vector<vector<CEdge*>> & pEdgeLinks)
{
	//����һ���������ռ�����������;
	vector<vector<CEdge*>> vEdgeLists;
	vEdgeLists.empty();

	CEdge* pEdge; 
	vector<CEdge*> pVctEdge;
	vector<CEdge*>::iterator  itr = this->m_edgesLink.begin();
	for(; itr < m_edgesLink.end(); itr++)   //һ�α�������path��
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

	//�ж�pVctEdge�Ƿ�Ϊ�գ���Ϊ������Ҫ���룻���һ�α��������⴦���ڴ�;
	if(pVctEdge.size() != 0)
	{
		vEdgeLists.push_back(pVctEdge);
		pVctEdge.clear();
	}

	//��ȡ����·������pEdgeLinks
	vector<vector<CEdge*>>::iterator itrlist = vEdgeLists.begin();
	for(; itrlist != vEdgeLists.end(); itrlist++)
	{
		pEdgeLinks.push_back((vector<CEdge*>)(*itrlist));
	}
}


//�ж�ĳ�����Ƿ��ص���Ϊ����;
//����˵���˱���ȡ�ص��ߺ�ʣ��һ����;
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


//��ȡarclink��һ�ε���ʼ�����.
//ȡ��һ�Σ��ڶ��Σ��Ա���ͬ���㣬�ó���ʼ�����;
bool 
CArcLink::getStartPointIndex(OUT int& iIndex) 
{
	iIndex = -1;

	//���
	int a1 = 0; //��һ��index1;
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


//��ȡarclink���һ�ζε��յ����.
//ȡ������һ�Σ������ڶ��Σ��Ա���ͬ���㣬�ó���ʼ�����;
//����false������ʧ����;
bool
CArcLink::getEndPointIndex(OUT int& iIndex)
{
	iIndex = -1; //initialize

	//���
	int a1 = 0; //������һ��index1;
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


//����arclink��һ�κ����һ���Ƿ�պϣ���ȡ���ص���֮��
//step1�����Ե�һ�κ����һ���Ƿ�Ϊ�գ���ȡ�ص��ߣ���
//step2�����������յ��Ƿ��غϣ��������Ƿ�պ�);
//���أ�true���պϣ� false�����պ�;
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

	//�жϵ�һ������ȡ�ص��ߺ��Ƿ���ʣ���;
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

	//������
	int nStartIndex = -1;
	if(!getStartPointIndex(nStartIndex))
	{
		return false;
	}
	//�յ����
	int nEndIndex = -1;
	if(!getEndPointIndex(nEndIndex))
	{
		return false;
	}
	//�Ƚ�;
	if(nStartIndex == nEndIndex)
	{
		return true;
	}
	return false;
}


//��arclink��һ�κ����һ���������ģ��ϲ�֮;
//���룺vector<vector<CEdge*>> & pEdgesLinks �Ѿ���ȡ�õĸ���������;
//*pEdgesLinks���ܺ����ص��߹��ɵ�������;�������;
//���أ�falseû���ϲ�;
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


//�����õĺ������������ɾ��; 
void 
CArcLink::testPintOriginalPath() //��ӡ��ͼ������ȱ�����õ���·��;  
{
#ifdef DEBUG_TO_PL_PRINT
	//CEdge* pEdge;
	acutPrintf(_T("·����ʼ��\n"));
	vector<CEdge*>::iterator itr = m_edgesLink.begin();
	for(; itr < m_edgesLink.end(); itr++)
	{
		acutPrintf(_T("���㣺(%d,%d)\n"),(*itr)->index1,(*itr)->index2);
	}

	acutPrintf(_T("·��������\n"));
#endif //DEBUG_TO_PL_PRINT
}

//=============CArcLink��========================end================;






//=============CArcLinkArray��========================================start=======
//����ʵ�ʵı�ʵ��ͼ��������ĳ��·���Ƿ�Ӧ�ã�1.�ظ����ɣ�2.���ѳɶ����ߣ���ʵ�ģ����ȵȡ�
//�˺������ܼ������path�����������ܷ�ʵ�����ɶ����ߵ�path��

//���캯��
CArcLinkArray::CArcLinkArray()
{
	;
}


CArcLinkArray::~CArcLinkArray()
{
	;
}


//�������ʵ��·����
//����ȡ��·�� �������ʣ�µıߣ���ȡ�ǻ�·��
bool
CArcLinkArray::correctByRealEdges()
{
	vector<CArcLink>::iterator itr = this->m_edgeLinkArray.begin();	

	//��carclink��������ȥ��closed path�зǻ����ֽ��а���;
	//for(; itr < m_edgeLinkArray.end(); itr++)
	//{
	//	if((*itr).isPathClosed())
	//	{
	//		(*itr).correctLinks(this);
	//	}
	//}

	//�ǻ�·��ȡ;
	for(itr = m_edgeLinkArray.begin(); itr < m_edgeLinkArray.end(); itr++)
	{
		//(*itr).numOfLoops(); //��ȡcarclink�ڲ���ʵloop��������Ϊcarclink��CEdge�����Ѿ��䶯�ˣ�
		//(*itr).extractLoops(m_edgeLinkArrayToPolyline);
		(*itr).extractLoopsOfOverlappedEdges(m_edgeLinkArrayToPolyline);
		(*itr).extractPathNotClosed(m_edgeLinkArrayToPolyline);
	}
	return true;
}


//��β�Ͳ��������
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


//����β�ͱ�����Ԫ�أ�
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


////�Ի���·����closed path��������ȡ��������ȡ�� ������carclinkѭ��һ�飻
////����closed path ��ȡ������arclink�ٽ�����ȡpath�� �ٶ�����carclinkѭ��һ�飻
//void
//CArcLinkArray::extractLoops()
//{
//	vector<CArcLink*>::iterator itr = this->m_edgeLinkArray.begin();
//	for(; itr < m_edgeLinkArray.end(); itr++)
//	{
//		itr->numOfLoops();
//		itr->extractLoops(m_edgeLinkArrayToPolyline);  //��ȡ������û�л����򲻻���ȡ��
//	}
//
//	for(; itr < m_edgeLinkArray.end(); itr++)
//	{
//		itr->numOfLoops();
//		itr->extractPathNotClosed(m_edgeLinkArrayToPolyline);  //��ȡ�ǻ�·������û��·�����򲻻���ȡ��
//	}
//}


////����
////���ɶ����ߣ�
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


////ͨ��CEdge p1��p2��ȡ��һ�������vertexֵ��
////��p1���������vertex����Ҫ�أ�point��startwidth��endwidth��bugle��index��
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


//·���������֮�󣬽�·�����ɶ����ߣ�
void
CArcLinkArray::toPolylines()
{
	CToPolyline objToPl;
	vector<CEdge*>* pedgelink;
	vector<vector<CEdge*>>::iterator itr = m_edgeLinkArrayToPolyline.begin();
	for(;itr < m_edgeLinkArrayToPolyline.end(); itr++)
	{
		AcDbPolyline* pline = new AcDbPolyline();

		objToPl.inputEdgelink((vector<CEdge*> *)(&(*itr))); //�������; 
		objToPl.to_polyline(pline);
		//m_vecPolylines.push_back(pline);
		join2database(pline);  //�������ݿ⣻���ռ�������;

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


//���Ժ�������ӡ·����
void
CArcLinkArray::testPrintOriginalPath()
{
#ifdef DEBUG_TO_PL_PRINT
	acutPrintf(_T("\nHere is CArcLinkArray::testPrintOriginalPath()---start:"));
	int iNum = 0; 
	vector<CArcLink>::iterator itr = m_edgeLinkArray.begin();
	for(; itr != m_edgeLinkArray.end(); itr++)
	{
		acutPrintf(_T("\n��%d��·����\n"),iNum);
		(*itr).testPintOriginalPath();
		iNum++;
	}
	acutPrintf(_T("\nHere is CArcLinkArray::testPrintOriginalPath()---end!"));

#endif  //#ifdef DEBUG_TO_PL_PRINT
}


//���Դ�ӡ�������ɶ����ߵ�m_edgeLinkArrayToPolyline���ݣ�
void 
CArcLinkArray::testm_edgeLinkArrayToPolyline()
{
#ifdef DEBUG_TO_PL_PRINT
	int i = 0;
	int inum = m_edgeLinkArrayToPolyline.size();
	acutPrintf(_T("\nCArcLinkArray::testm_edgeLinkArrayToPolyline():��ȡ��%d���������ɶ����ߵ���ʵ·����"),inum);
	vector<vector<CEdge*>>::iterator itrA = m_edgeLinkArrayToPolyline.begin();
	vector<CEdge*>::iterator itrB;
	for(; itrA < m_edgeLinkArrayToPolyline.end(); itrA++)
	{
		acutPrintf(_T("��%d���������ɶ����ߵ���ʵ·��."),i);
		itrB = (*itrA).begin();
		for(; itrB < (*itrA).end(); itrB++)
		{
			acutPrintf(_T("\n�������(%d,%d)"),(*itrB)->index1,(*itrB)->index2);
		}
		i++;
	}
#endif //#ifdef DEBUG_TO_PL_PRINT
}


//=============CArcLinkArray��========================end================;



//=============CToPolyline��========================start================;
//���캯��
CToPolyline::CToPolyline(vector<CEdge*>*& pEdgeLink,IN const AcGeTol equalPointTol)
{
	m_isclosed = false;

	if(pEdgeLink != NULL)
	{
		m_edgeLink = pEdgeLink;
	}

	m_equalPoint = equalPointTol;
}


//���캯��2
CToPolyline::CToPolyline()
{
	m_isclosed = false;
	m_startPointIndex = -1;  //����ʼ��һ�¼���;
}


//��������
CToPolyline::~CToPolyline()
{	
}


//����·�����������ɶ����ߵ�)
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


//�������ֵ��
void
CToPolyline::inputEquPntTol(IN const AcGeTol equalPointTol)
{
	m_equalPoint = equalPointTol;
}



//�������ɺ��ж��Ƿ�պ�;
bool
CToPolyline::isVetexListClosed()
{
	//ȡ�����ĵ�һ�����㣬�����һ������;
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


//�ӱ߱��ȡ��һ������;
void 
CToPolyline::getFirstPosition()
{
	AcDb2dVertex* pVertex2d;

	//ȡ��һ��edge
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
		m_vertexArr.push_back(pVertex2d);  //��һ�����㣻
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
	

	//������CEdge�����ӵ㣻	
	if(pt1start.isEqualTo(pt2start) || pt1start.isEqualTo(pt2end))
	{//pt1start�����ӵ㣻���һ��������pt1end;
		if(pEdgeFirst->enttype == AcGe::kLine2d)
		{
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			m_vertexArr.push_back(pVertex2d);  //��һ�����㣻
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //Բ����
		{
			//��һ���㣻
			AcDbArc* pArc = (AcDbArc*)pEdgeFirst->ptrEnt;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(getArcBugle(pArc,false));
			m_vertexArr.push_back(pVertex2d); 
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kPolyline2d)
		{
			//�����һ�����㣻
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
			m_vertexArr.push_back(pVertex2d);  //��һ�����㣻		
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //Բ����
		{
			//��һ���㣻
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
			//�����һ�����㣻
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
		acutPrintf(_T("ERROR��CToPolyline class::getFirstVertex()003"));
	}
}


//ȡ�õ�һ����������; 
//���·���պϣ���˶����Ǽٶ��ĵ�һ������;
//�Ƚ�2��edge����ŽӲ���ϵ���Ӷ�ȷ����ʼ��;
//m_startPointIndex���ϸ���;
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
			acutPrintf(_T("\n������.......getFirstVertexIndex()-001"));			
		}
	}
	else if(m_edgeLink->size() == 1)  //ֻ��һ���ߣ����ָ��һ������Ϊ���;
	{
		pEdgeFirst = (*m_edgeLink)[0];	
		m_startPointIndex = pEdgeFirst->index1;			
	}
	else
	{
		//��������ˣ�˵���쳣����.
		acutPrintf(_T("\n������.......getFirstVertexIndex()-002"));	
	}
}


//ȡ��һ����������; 
//���ߵ���1������ȡindex1������Լ���� ��Ϊȡĩ�˵����ȡindex2��
//����������2�������պϣ�����׼ȷ; ���պϣ�����make����vertex��
//����������2�����׼ȷ;
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
			if(index2 == index3 || index2 == index4)  //˵����2�λ��պ���;
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
	else  //������!
	{
		return -1;	
	}
}


//ȡ���һ����������
//���ߵ���1�����һ���˵����ȡindex2������Լ������һ���˵�ȡindex1��
//����������2�������պϣ�����׼ȷ; ���պϣ�����Ҫ���Ķ˵����;
//����������2�����׼ȷ;
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
			if(index2 == index3 || index2 == index4)  //˵����2�λ��պ���;
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
	else  //�����ˣ�
	{
		return -1;	
	}
}


//��һ��ֱ�߱���ȡ����vertex��
void
CToPolyline::insertLine(IN const CEdge* pEdge)
{
	AcGePoint3d ptStart;
	//�ж����;
	if(pEdge->index1 == m_startPointIndex)
	{
		ptStart = pEdge->ptstart;
		m_startPointIndex = pEdge->index2;  //����������;
	}
	else if(pEdge->index2 == m_startPointIndex)
	{
		ptStart = pEdge->ptend;
		m_startPointIndex = pEdge->index1; //����������;
	}
	
	//���vertex;
	AcDb2dVertex* vtxNow = new AcDb2dVertex();
	vtxNow->setPosition(ptStart); 
	vtxNow->setBulge(0);
	m_vertexArr.push_back(vtxNow);
	printVertex(vtxNow); //��ӡ������Ϣ,������;
}


//�Ի���ʵ����ȡ����
void
CToPolyline::insertArc(IN const CEdge* pEdge)
{
	AcGePoint3d ptStart;
	bool isUnClockwise = true; //�����������ʱ��bugle�Ƿ�Ҫȡ��;
	double dblBugle = 0;
	//�ж����;
	if(pEdge->index1 == m_startPointIndex)
	{
		ptStart = pEdge->ptstart;
		isUnClockwise = true;
		m_startPointIndex = pEdge->index2;  //����������;
	}
	else if(pEdge->index2 == m_startPointIndex)
	{
		isUnClockwise = false;
		ptStart = pEdge->ptend;
		m_startPointIndex = pEdge->index1; //����������;
	}

	//���vertex;
	AcDb2dVertex* vtxNow = new AcDb2dVertex();
	vtxNow->setPosition(ptStart); 
	dblBugle = getArcBugle((AcDbArc*)(pEdge->ptrEnt),isUnClockwise);
	vtxNow->setBulge(dblBugle);
	m_vertexArr.push_back(vtxNow);
	printVertex(vtxNow); //��ӡ������Ϣ,������;
}


//����һ�������ߣ�������ȡ����;
void
CToPolyline::insertPolyline(IN const CEdge* pEdge)
{
	bool isUnClockwise = true; //�����������ʱ��bugle�Ƿ�Ҫȡ��;

	//�ж����;
	if(pEdge->index1 == m_startPointIndex)
	{
		isUnClockwise = true;
		m_startPointIndex = pEdge->index2;  //����������;
		insertPolyline((AcDbPolyline*)(pEdge->ptrEnt),isUnClockwise);
	}
	else if(pEdge->index2 == m_startPointIndex)
	{
		isUnClockwise = false;
		m_startPointIndex = pEdge->index1; //����������;
		insertPolyline((AcDbPolyline*)(pEdge->ptrEnt),isUnClockwise);
	}
	else
	{
		acutPrintf(_T("\n������.......insertPolyline()-001"));
		return;
	}
}


//���ݱ߱��еı߼�������߶���;
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
		insertPolyline(pEdge); //��������ԭ��˳����롣
	}
	else
	{
		acutPrintf(_T("ERROR��CToPolyline class::getNextVertex()003"));
	}	
}


//1.����������Գɻ��Ρ���ô����
//2.��������߲��ɻ��Σ� ��ô����
//3.���һ���ߵĽ���������δ���
//��ÿ�λ�ȡ2�����㣬��һ��������������AcDb2dVertex���ڶ���û�м���bugleֵ������ָʾ��һ���ߵ����ӵ㣬Ȼ�����bugleֵ��
//    ���һ��������Ҫ�ر�����Ҫ�ж��Ƿ�����߱պϣ�
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

	//������CEdge�����ӵ㣻	
	if(pt1start.isEqualTo(pt2start) || pt1start.isEqualTo(pt2end)) 
	{//pt1start�����ӵ㣻���һ��������pt1end;
		if(pEdgeFirst->enttype == AcGe::kLine2d)
		{
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			//vtx2d.setBulge(0);
			//vtx2d.setStartWidth(0);
			//vtx2d.setEndWidth(0);
			//vtx2d.setTangent(0);		
			m_vertexArr.push_back(pVertex2d);  //��һ�����㣻
			//�ڶ�������;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //Բ����
		{
			//��һ���㣻
			AcDbArc* pArc = (AcDbArc*)pEdgeFirst->ptrEnt;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(getArcBugle(pArc,false));
			m_vertexArr.push_back(pVertex2d); 
			//�ڶ����㡣
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kPolyline2d)
		{
			//�����һ�����㣻
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
			
			//����ڶ������㣻
			AcDbPolyline* pline = (AcDbPolyline*)(pEdgeFirst->ptrEnt);
			insertPolyline(pline,true);  //����ptrEntԭ���Ķ���˳�򣬲����¶����ߣ�
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
			m_vertexArr.push_back(pVertex2d);  //��һ�����㣻
			//�ڶ�������;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kCircArc2d)  //Բ����
		{
			//��һ���㣻
			AcDbArc* pArc = (AcDbArc*)pEdgeFirst->ptrEnt;
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1start);
			pVertex2d->setBulge(getArcBugle(pArc,true));
			m_vertexArr.push_back(pVertex2d);  
			printVertex(pVertex2d);
			//�ڶ����㡣
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
		}
		else if(pEdgeFirst->enttype == AcGe::kPolyline2d)
		{
			//�����һ�����㣻
			pVertex2d = new AcDb2dVertex();
			pVertex2d->setPosition(pt1end);
			pVertex2d->setBulge(0);
			m_vertexArr.push_back(pVertex2d);
			printVertex(pVertex2d);
			//����ڶ������㣻
			AcDbPolyline* pline = (AcDbPolyline*)pEdgeFirst->ptrEnt;
			insertPolyline(pline,false);  //����ptrEntԭ���Ķ���˳�򣬲����¶����ߣ�
		}
		else
		{
			acutPrintf(_T("CToPolyline class::getFirstVertex()002"));
		}
	}
	else
	{
		acutPrintf(_T("ERROR��CToPolyline class::getFirstVertex()003"));
	}
}


//��һ�����㴦�����֮���Ѿ�֪���ڶ�������position������
//�˺�����õڶ�������bugle����������֮��Ȼ����õ��������㣨���޼���bugle������
void
CToPolyline::getNextVertex(CEdge* pedge)
{
	AcDb2dVertex* vtx2dLast = m_vertexArr.back(); //��һ���㣻
	AcDb2dVertex* vtxNow;
	AcGePoint3d ptLast(vtx2dLast->position());

	//�ж���㣻
	//pedge�����Ϊ���ӵ㣬�����Ƿ�Ҫ�Ըû��η���
	if(ptLast.isEqualTo(pedge->ptstart))
	{
		if(pedge->enttype == AcGe::kLine2d)
		{
			//��һ��vertex�����޸��ˣ�
			//��ӵڶ������㣻
			vtxNow = new AcDb2dVertex();
			vtxNow->setPosition(pedge->ptend);			
			m_vertexArr.push_back(vtxNow);
			printVertex(vtxNow);
		}
		else if(pedge->enttype == AcGe::kCircArc2d)
		{
			//��һ��vertex��Ҫ�޸�bugle��
			double fbugle = getArcBugle((AcDbArc*)pedge->ptrEnt,true);
			vtx2dLast->setBulge(fbugle);
			m_vertexArr.pop_back();  //������
			m_vertexArr.push_back(vtx2dLast);  //�ټ��룻
			//��ӵڶ������㣻
			vtxNow = new AcDb2dVertex();
			vtxNow->setBulge(0);
			vtxNow->setPosition(pedge->ptend);
			m_vertexArr.push_back(vtxNow);
			printVertex(vtxNow);
		}
		else if(pedge->enttype == AcGe::kPolyline2d)
		{
			insertPolyline((AcDbPolyline*)pedge->ptrEnt,false); //��������ԭ��˳����롣
		}
		else
		{
			acutPrintf(_T("ERROR��CToPolyline class::getNextVertex()003"));
		}
	}
	else if(ptLast.isEqualTo(pedge->ptend))
	{
		if(pedge->enttype == AcGe::kLine2d)
		{
			//��һ��vertex�����޸��ˣ�
			//��ӵڶ������㣻
			vtxNow = new AcDb2dVertex();
			vtxNow->setPosition(pedge->ptstart);
			m_vertexArr.push_back(vtxNow);
			printVertex(vtxNow);
		}
		else if(pedge->enttype == AcGe::kCircArc2d)
		{
			//��һ��vertex��Ҫ�޸�bugle��
			double fbugle = getArcBugle((AcDbArc*)pedge->ptrEnt,false);
			vtx2dLast->setBulge(fbugle);
			m_vertexArr.pop_back();  //�ȵ�����
			m_vertexArr.push_back(vtx2dLast);  //�ټ��룻
			//��ӵڶ������㣻
			vtxNow = new AcDb2dVertex();
			vtxNow->setBulge(0);
			vtxNow->setPosition(pedge->ptstart);
			m_vertexArr.push_back(vtxNow);
			printVertex(vtxNow);
		}
		else if(pedge->enttype == AcGe::kPolyline2d)
		{
			insertPolyline((AcDbPolyline*)pedge->ptrEnt,true); //��������ԭ��˳����롣
		}
		else
		{
			acutPrintf(_T("ERROR��CToPolyline class::getNextVertex()003"));
		}
	}
	else
	{
		acutPrintf(_T("ERROR��CToPolyline class::getNextVertex()004"));
	}
}


//�������һ�������vertex
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
		if(indexLast == pEdge->index1)  //�����߶�����Ҫ����;
		{
			//����pt3d��bugle��Startwidth...
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


//�����һ������AcDbVertex2d��
//�����������ɵĶ������Ƿ�պϣ�
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


//�ж�·���Ƿ�պ�;
//ȡ��β������ţ��ж�����Ƿ�Ϊ2��
bool
CToPolyline::isPathClosed()
{
	//ȡ��һ�������һ��������ţ��ж��Ƿ���ȣ� ��ȴ���պ�;
	int nNum = m_edgeLink->size();
	int index1 = getFirstVertexIndex(); 
	int index2 = getLastVertexIndex();
	if(nNum > 1)
	{
		if(-1 == index1)
		{
			return true;  //˵����2������ɵģ��պ�;
		}
		else
		{
			if(index1 == index2)
			{
				return true;  //�պ�;
			}
			else
			{
				return false;  //���պϣ�
			}
		}
	}
	else
	{
		return false;  //���պ�;
	}
}


//��CEdgeϵ�У�·����ת��Ϊ�����߶���ϵ�У�
void
CToPolyline::to_vertex2d()
{
	//���
	m_vertexArr.clear();

	//���߱��Ƿ�Ϊ��;
	if(m_edgeLink->size() == 0)
	{
		return;
	}

	//ȡ��һ��������ţ�
	getStartVertexIndex();

	vector<CEdge*>::iterator itr = m_edgeLink->begin();
	for(; itr != m_edgeLink->end(); itr++)
	{
		transEdgeToVetex((CEdge*)(*itr));
	}

	//�������һ�����㣻���һ������Ҫ���⴦��;
	//�ж��Ƿ�պ�; ������պϣ���������һ�������vertex������Ҫbugle;
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



//AcDbArcʵ�壺������ʱ��ת���ģ����ܳ�ʼ��λ�ͼ;
//���ݻ�ʵ�壬�������Ϊ��������ɻ��κ��bugle��
//isFromStartPoint��ʾ���������Ǵ������bugle�����Ǵ��յ����bugle��
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


//���Ժ�������ӡ���㣻
void
CToPolyline::printVertex(IN AcDb2dVertex*& pt)
{
#ifdef DEBUG_TO_PL_PRINT
	AcGePoint3d pt1;
	double dblBugle;

	pt1 = pt->position();
	dblBugle = pt->bulge();

	acutPrintf(_T("position is %f��%f  bugle is %f\n"),pt1.x,pt1.y,dblBugle);
#endif
}


//���Ժ�������ӡ�߱�;
void
CToPolyline::printEdgeLink()
{
#ifdef DEBUG_TO_PL_PRINT
	if(m_edgeLink->size() == 0)
	{
		return;
	}

	vector<CEdge*>::const_iterator itr = m_edgeLink->begin();
	acutPrintf(_T("\n��·����Ϣ����:\n"));
	for(; itr != m_edgeLink->end(); itr++)
	{
		//����Ƿ���NULLָ��
		if((CEdge*)(*itr) == NULL)
		{
			acutPrintf(_T("********************************************************************"));
			acutPrintf(_T("********************************************************************"));
			acutPrintf(_T("********************************************************************"));
			continue;
		}
		//acutPrintf(_T("\n�������(%d,%d)"),((CEdge*)(*itr))->index1,((CEdge*)(*itr))->index2);
		//acutPrintf(_T("��������(%f,%f)"),((CEdge*)(*itr))->ptstart.x,((CEdge*)(*itr))->ptstart.y);
		//acutPrintf(_T("��������(%f,%f)\n"),((CEdge*)(*itr))->ptend.x,((CEdge*)(*itr))->ptend.y);
		acutPrintf(_T("\n�������(%d,%d)"),(*itr)->index1,(*itr)->index2);
		acutPrintf(_T("��������(%f,%f)"),(*itr)->ptstart.x,(*itr)->ptstart.y);
		acutPrintf(_T("��������(%f,%f)\n"),(*itr)->ptend.x,(*itr)->ptend.y);
	}
	m_edgeLink;

#endif  //#ifdef DEBUG_TO_PL_PRINT
}




//��ĳ����CEdge�Ƕ����ߣ����轫�����߶�����һ�����µĶ����ߡ�
void
CToPolyline::insertPolyline(IN AcDbPolyline* pLine,bool isReversed)
{
	int numVtx = pLine->numVerts();  //�������;
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
	//	pLine->reverseCurve();  // �������ߵߵ������� //Ϊɶ���Ǳ���notopenforwirte��������
	//	pLine->downgradeOpen();
	//}		
	if(!isReversed)  //��������Ҫ�ߵ�;
	{
		for(int i = numVtx-1 ; i > 0; i--)  //���һ���������ﲻ��Ҫ����,���⴦��;
		{			
			//ע�⣬���ݶ��ǵߵ�������.
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
		for(int i = 0 ; i < numVtx - 1; i++)  //���һ���������ﲻ��Ҫ����
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
				acutPrintf(_T("ERROR��CToPolyline::insertPolyline()����--001"));
				continue;
			}*/
		}
	}	
}


//�����ɵĶ���AcDb2dVertexϵ�����ɶ����ߣ�
void
CToPolyline::to_polyline(AcDbPolyline*& pLine)
{
	//��CEdgeϵ�мӹ��ɶ���ϵ�У�
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

	//�ж϶������Ƿ�պ�;	
	if(m_isclosed)
	{
		pLine->setClosed(Adesk::kTrue);  //�պ�֮��
	}
}

//=============CToPolyline��========================end================;








//=============ͼ��========================================start;

//���캯��������ͼ����ͼ�洢���б�ʵ�壻
CGraphEnts::CGraphEnts()
{
	numEdges = 0;
	numVertexs = 0;
	m_ssLength = -1;
	m_nDotNum = -1;
}


//��������
CGraphEnts::~CGraphEnts()
{
	//�ͷű�ռ�õ��ڴ棻
	vector<CEdge* > ::iterator itrEdges = m_vctEdges.begin();
	for(; itrEdges < m_vctEdges.end(); itrEdges++)
	{
		CEdge* pedge = (CEdge*)(*itrEdges);
		if(pedge != NULL)
		{
			pedge->ptrEnt->close();  //�رմ򿪵�ʵ��;
			delete pedge;
		}
	}

	//�ͷŶ���ռ�õ��ڴ棻
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

	//�ͷ�visited��
	//delete[] visited;
}



//����һ����CEdge���󣻶����ж������Ϊ�գ���Ϊ��δ��ֵ��
//��insertEdge()���ã�
bool 
CGraphEnts::createEdge(CEdge*& pedge,ads_name ssunit)
{
	//pedge = new CEdge();  //�½�һ���ߣ�	˭��˭������

	AcDbObjectId id;
	acdbGetObjectId(id,ssunit);
	AcDbEntity* pent;
	acdbOpenObject(pent,id,AcDb::kForRead);

	AcGe::EntityId enttype;	
	if(pent->isA() == AcDbLine::desc())
	{
		AcDbLine *plineseg = (AcDbLine*)pent;
		pedge->enttype = AcGe::kLine2d;  //�߶����ͣ�
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
		pedge->enttype = AcGe::kCircArc2d;  //�߶����ͣ�
		//pedge->m_numSameEdges = 0;
		//pedge->path1 = NULL;
		//pedge->path2 = NULL;
		//pedge->index1 = -1;  //�ҵ��������û�У�
		//pedge->index2 = -1;   // �ҵ��������2û�У�
		pArcseg->getEndPoint(pt3d);
		pedge->ptend.set(pt3d.x,pt3d.y,pt3d.z);
		pArcseg->getStartPoint(pt3d);
		pedge->ptstart.set(pt3d.x,pt3d.y,pt3d.z);
		//pedge->ptrSameEdges = NULL;
	}
	else if(pent->isA() == AcDbPolyline::desc())
	{
		AcDbPolyline *pline = (AcDbPolyline*)pent;
		if(pline->isClosed() == kTrue) //�պ϶����߲���������.
		{
			pent->close();
#ifdef DEBUG_TO_PL_PRINT
			acutPrintf(_T("\n�պ϶����ߣ�CGraphEnts.createEdge"));
#endif
			return false;
		}

		AcGePoint3d pt3d;
		pedge->enttype = AcGe::kPolyline2d;  //�߶����ͣ�
		//pedge->m_numSameEdges = 0;
		//pedge->path1 = NULL;
		//pedge->path2 = NULL;
		//pedge->index1 = -1;  //�ҵ��������û�У�
		//pedge->index2 = -1;   // �ҵ��������2û�У�
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
		acutPrintf(_T("\n�߶����Ͳ��԰���CGraphEnts.createEdge"));
#endif
		
		return false;
	}

	//���붥�㣬���ض������;  
	int indexStart = insertVertex(pedge->ptstart);
	int indexEnd = insertVertex(pedge->ptend);
	pedge->index1 = indexStart;
	pedge->index2 = indexEnd;
	pedge->path1 = NULL;
	pedge->path2 = NULL;
	pedge->m_leftSameEdges = 1;  //���ڴ������µ���ͬ����ߵ������������Լ�.��;������;
	pedge->m_numSameEdges = 1;  //���ڴ������µ���ͨ����ߵ������������Լ�����;����ʱû���ϣ�
	pedge->ptrSameEdges = NULL;
	pedge->ptrEnt = pent;

	pent->close();	
	return true;
}



//Ѱ��ͼ��ĳ���ߣ����رߵ�ָ�룻
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
			pedge = pedge->path1;  //��path1������
		}
	}

	return NULL;
}



//����pedge��ͬ�ıߣ���������ͬ�Ķ��㼰��ţ�
//pedge�ǻ�û����ͼ��������֪�䶥����ż����㣻
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
				pedgeTemp = pedgeTemp->path2;  //��path2������
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
				pedgeTemp = pedgeTemp->path2;  //��path2������
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
				pedgeTemp = pedgeTemp->path1;  //��path2������
			}
		}
		else
		{
			return NULL;
		}
	}  //while
	return NULL;
}



//ͳ��ĳ���ڵ�Ķȣ�
//��Ҫ���ڵ���ͼ����:���ﲻ����ȣ�
int 
CGraphEnts::getDegreeOfVertex(IN int index)
{
	CEdge* pEdge;
	int idegree = 0; //ͳ�ƶ�����
	pEdge = m_vertexTable[index]->adj;
	while(pEdge != NULL)
	{
		idegree++;
		pEdge = (index == pEdge->index1)?pEdge->path1:pEdge->path2;
	}
	return idegree;
}


//����ڵ㣻
//���ز���Ķ������ţ������Ѵ��ڵĶ�����ţ�
int
CGraphEnts::insertVertex(AcGePoint3d& pt)
{
	int index;
	index = findVertexNode(pt);
	if(index == -1)  //�鲻���иö�������ڶ������;
	{
		CVertex* pobjVertex = new CVertex(pt);  //��������;
		m_vertexTable.push_back(pobjVertex); //�ռ�;
		
		numVertexs++;
		index = numVertexs-1;  //������Ŵ�0��ʼ��
		//�����ķ��ʱ�־;
		visited.push_back(0);

		//�����꼰��ż���map;
		m_pointMap.insert(pt,index);

		return index; 
	}
	else  //�ҵ��˸õ�����ڶ�����С��򲻲���õ㣬����������ţ�
	{
		return index;
	}
}



//���Ժ�������ӡ�ߵ���Ϣ��
void
CGraphEnts::printEdgeInfo(CEdge* pEdge)
{
#ifdef DEBUG_TO_PL_PRINT
	if(pEdge == NULL)
	{
		acutPrintf(_T("�˱�CEDGEΪ��ָ�룬�˳���\n"));
		return;
	}
	else
	{
		acutPrintf(_T("\n�߶������(%d,%d)"),pEdge->index1,pEdge->index2);
		acutPrintf(_T("������꣨%f,%f)"),pEdge->ptstart.x,pEdge->ptstart.y);
		acutPrintf(_T("�յ����꣨%f,%f)\n"),pEdge->ptend.x,pEdge->ptend.y);
		return;
	}
#endif   //#ifdef DEBUG_TO_PL_PRINT
}


//����һ���ߣ�
//
void
CGraphEnts::insertEdge(CEdge* pedge)
{
	CEdge* pEdgeFind = NULL;
	pEdgeFind = findEdge(pedge); //Ѱ����ͬ����ıߣ�
	if(pEdgeFind != NULL)  //�ҵ�����ͬ���ص��ߣ������ص�����;
	{
		#ifdef DEBUG_TO_PL_PRINT
		acutPrintf(_T("\n�ҵ�����ͬ���ص��ߣ������ص�����:"));
		#endif   //#ifdef DEBUG_TO_PL_PRINT

		CEdge* pedgeTemp = pEdgeFind;
		while(pedgeTemp->ptrSameEdges != NULL)
		{
			pedgeTemp = pedgeTemp->ptrSameEdges;  
		}
		pedgeTemp->ptrSameEdges = pedge; //�µı߲��뵽����ĩ��;   
		pedge->ptrSameEdges = NULL;

		pEdgeFind->m_leftSameEdges++;
		pEdgeFind->m_numSameEdges++;
	}
	else    //û���ص��ߣ�����Ҫ�����Ϊ�µıߣ�
	{
		#ifdef DEBUG_TO_PL_PRINT
		acutPrintf(_T("\nû���ص��ߣ�����Ҫ�����Ϊ�µı�:"));
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

	//������Ϣ����ӡ�ߵ���Ϣ�����������ɾ��;
	#ifdef DEBUG_TO_PL_PRINT
	acutPrintf(_T("\n����һ����:"));
	printEdgeInfo(pedge);
	#endif   //#ifdef DEBUG_TO_PL_PRINT	
}



//ͼ�Ĳ���֮һ��ɾ��һ����
bool
CGraphEnts::delEdge(CEdge* pedge)
{
	int index1 = pedge->index1;
	int index2 = pedge->index2;

	//�ҵ�����ָ��->index1;	
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
	//�Ͽ�index1����;
	if(pEdgeTmp != NULL)
	{
		pEdgeTmp = pEdgeTmp->path1->path1;
		pEdgeTmp->path1 = NULL;
	}
	else
	{
		return false;
	}


	//�ҵ�����ָ��->index2;	
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
	//�Ͽ�index1����;
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



//Ѱ�ҽڵ㣬�����ҵ��Ľڵ���ţ����Ҳ���������-1��
//�������; 
//ע�⣺��ͼ���滹û�ж����ʱ��Ҫ�ж�֮;
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



//��ȡv1�ĵ�һ���ڽӶ��㣻�����䶥����ţ�
//��ȡ�ڽӶ����˳���ȡ����������ıߣ�
////////////////1.��Ϊ�����Ƕ����ڽӱ�����Ҫ���ϸ��׽ڵ������ҵ�һ���ڽӵ㣻
////////////////2.�����һ���ڽӵ��ֻ���ҵ����ڵ㣬��ѭ���ˣ�
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




// ��ȡ����v1���ڽӵ�v2��һ���ڽӶ��㣻
int
CGraphEnts::GetNextNeighbor(const int v1,const int v2,CEdge*& pedge)
{
	//int index1;
	//int index2;
	if(v1 == v2)
	{//�˴�û��Ҫ��
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
			//�������⣬��һ����ô��;
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
			else  //������һ���ߣ�
			{
				pedge = (v1 == pedge->index1)?(pedge->path1):(pedge->path2);
			}
		}
	}	
	return -1;
}



//����visited���飻
void
CGraphEnts::resetvisited()
{
	vector<int>::iterator itr = visited.begin();
	for(; itr != visited.end(); itr++)
	{
		(*itr) = 0;
	}
}


//��ȱ���ͼ��
//���������л�ȡ��·����ȡ������closed polyline��
//Ҳ��ȡ���պϵ�·��������not closed polyline��
//�����õݹ鷽ʽ��Ϊʲô������Ϊ�ݹ�������жϲ��ô����ǵݹ�Ҳͦ�򵥵ģ�
//����ջ�ṹ����ڵ㣬�ǵݹ���ȱ���������ȡ��·��
void
CGraphEnts::DFS(int index)
{
	if(index >= this->numVertexs)  //index���Ϸ���
	{
		return;
	}

	stack<int>  stackVertex;  //����������ջ��
	stack<CEdge*> stackEdges;  //��Ż�·�ߣ�Ҳ��ջ��
	CEdge* pedge = NULL;  //ָ��ߵ�ָ�룻

	visited[index] = 1;  
	stackVertex.push(index);
	m_stackEdges.push_back(index);  //�����CArcLink;��һ��������ջ��
	while(!stackVertex.empty())   //ѭ����ȱ�����
	{
		int iparent = stackVertex.top();  //ע�⣬ֻ��ȡ����Ԫ�أ����ǵ�����
		int ineighbor = iparent;		//����ֵ���ѣ���������ͼ��>=0���ɣ�	
		//GetFirstNeighbor()����ֵ�϶�>=0,������-1��GetNextNeighbor��һ���ˣ�	
		ineighbor = GetFirstNeighbor(iparent,pedge); 

		while(ineighbor != -1)
		{
			//GetFirstNeighbor()����ֵ�϶�>=0,������-1��GetNextNeighbor��һ���ˣ�
			//ineighbor = GetFirstNeighbor(iparent,pedge); 
			if(visited[ineighbor] == 0)
			{
				//��ջ����Ϊ�ýڵ�û�б����ʹ���
				m_stackEdges.push_back(pedge);  //����ջ;

				//����ineighbor;
				visited[ineighbor] = 1;
				stackVertex.push(ineighbor);  //��ջ��
				m_stackEdges.push_back(ineighbor);  //������ջ���˲���������ظ���^_^

				//ȡ��һ���ڽӶ��㣻
				//itemp = ineighbor;   //����֮�����ḳֵ��iparent���˴����ı�iparentֵ��  
				//ineighbor = GetFirstNeighbor(ineighbor,pedge);  //����iparent,ineighbor; 


				//�������ռ���·�����߷ǻ�·��·;  
				testIfLoopONextVtxNode(ineighbor);

				iparent = ineighbor; //��һ����
				ineighbor = GetFirstNeighbor(iparent,pedge);
			}
			else
			{
				ineighbor = GetNextNeighbor(iparent,ineighbor,pedge);	
			}
		} //while;
		stackVertex.pop(); //������:��Ϊ�˽ڵ���ڽӵ㶼�������ˣ����Ե�������
		m_stackEdges.pop_edge();
		m_stackEdges.pop_index();  //�������㣻
	} //while
}


//ͼ�����ж��㶼����ֹ��ˣ�ÿ������ֻ��1-2�ȣ�����ֱ����ȡһ���߱�;
//
void CGraphEnts::DFS2(int iIndex)
{
	//�ж϶����Ƿ񱻷��ʹ���
	if(iIndex < 0 || iIndex >= numVertexs || visited[iIndex] == 1)
	{
		#ifdef DEBUG_TO_PL_PRINT
		acutPrintf(_T("\n DFS2():numVertexs is %d ."),numVertexs);
		acutPrintf(_T("DFS2():vertex is %d "),iIndex);
		#endif
		return;
	}

	//���ö���Ķ�;
	int i = getDegreeOfVertex(iIndex);
	if(i > 2)
	{
		acutPrintf(_T("\n����%d,��Ϊ%d ������1����..."),iIndex,i);
		return;
	}

	//������Ϊ1�Ķ��㣻
	DFS(iIndex);
}


//��ȡ������; 
//ǰ���ǣ�ͼ�����ж���Ķ�С�ڵ���2;
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

	//��ȡ��һ��;
	vector<CEdge*> vEdgeLinksFirst;  //�ռ���һ�α�����;
	vector<CEdge*> vEdgeLinksSecond; //�ռ��ڶ��α�����;
	int iVtxIndex = iIndex; 
	int iParent = iIndex;
	int iNewVtx = -1;  //����»�ȡ�Ķ������;
	CEdge* pEdge = NULL;
	iVtxIndex = GetFirstNeighbor(iParent,pEdge);
	int iBak = iVtxIndex;
	while(isVertexIndexValid(iVtxIndex)) //�Ϸ��������;
	{
		visited[iVtxIndex] = 1;
		vEdgeLinksFirst.push_back(pEdge);
		iNewVtx = getAnotherVertex(iParent,iVtxIndex,pEdge);
		if(isVertexIndexValid(iNewVtx))
		{
			if(visited[iNewVtx] == 1)  //�����ʹ���;
			{
				vEdgeLinksFirst.push_back(pEdge);
				break;
			}
		}
		//����˳��;
		iParent = iVtxIndex;
		iVtxIndex = iNewVtx;
	}

	//��ȡ�ڶ���;
	iVtxIndex = -1; 
	iParent = iIndex;
	iNewVtx = -1;  //����»�ȡ�Ķ������;
	pEdge = NULL;
	iVtxIndex = GetNextNeighbor(iIndex,iBak,pEdge);
	if(isVertexIndexValid(iVtxIndex))
	{
		if(visited[iVtxIndex] == 0)
		{
			while(isVertexIndexValid(iVtxIndex)) //�Ϸ��������
			{
				visited[iVtxIndex] = 1;
				vEdgeLinksSecond.push_back(pEdge);
				iNewVtx = getAnotherVertex(iParent,iVtxIndex,pEdge);
				//����˳��;		
				iParent = iVtxIndex;
				iVtxIndex = iNewVtx;
			}
		}	
	}	

	//�ϲ�����;
	//�ϲ���һ��;
	CEdge* pEdgeTmp = NULL;
	std::reverse(vEdgeLinksFirst.begin(),vEdgeLinksFirst.end()); //����;
	vector<CEdge*>::iterator itr = vEdgeLinksFirst.begin();
	for(; itr != vEdgeLinksFirst.end(); itr++)
	{
		pEdgeTmp = (CEdge*)(*itr);
		m_stackEdges.push_back(pEdgeTmp);
	}
	//�ϲ��ڶ���;
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


//��ȱ�������ӡͼ�ı���Ϣ;
//�ݹ�;
//���ݹ����̫�࣬��ᵼ��cad������Ŀǰ�ݹ鵽12680��ҵ�cad; ���ù�ȱ�����.
void CGraphEnts::testDFS3(IN int iIndex)
{
	visited[iIndex] = 1;  //��ǳ��Ѿ����ʹ�;
	CEdge* pEdge = NULL;

	int iNextVtxIndex = GetFirstNeighbor(iIndex,pEdge);
	if(visited[iNextVtxIndex] == 0)
	{
		//��ӡ����Ϣ;
		printEdgeInfo(pEdge);
		testDFS3(iNextVtxIndex);
	}

	//������һ���ڽӱ�;
	do
	{
		iNextVtxIndex = GetNextNeighbor(iIndex,iNextVtxIndex,pEdge);
		if(iNextVtxIndex != -1)
		{
			if(visited[iNextVtxIndex] == 0)
			{
				//��ӡ����Ϣ;
				printEdgeInfo(pEdge);
				testDFS3(iNextVtxIndex);
			}
		}		
	}while(iNextVtxIndex != -1);
}


//�����ã���ӡͼ�����б���Ϣ;
void 
CGraphEnts::DFS3()
{
	resetvisited();  //���ж���ȫ������Ϊδ����״̬;

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


//ȡĳ����iParent�ĳ�v1�����һ���㣬ǰ���Ǹö����Ϊ2;
//���Ҳ���������-1;
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

	//ȡ��һ���ڽӵ�;
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


//���Զ�������Ƿ�Ϸ�;
bool
CGraphEnts::isVertexIndexValid(IN const int iVertexIndex)
{
	if(iVertexIndex < 0 || iVertexIndex > numVertexs)
	{
		return false;
	}
	return true;
}


//��ÿ������ȡ���ص��߹��ɵĻ���Ҳ����ȡ��·��
//ֱ�Ӱ����㼯��������?
bool
CGraphEnts::extractOverlappedEdges() 
{
	int iIdx = 0;   //ͼ�������
	CEdge* pEdge = NULL;  //ָ��ߵ���ʱָ��.
	//ѭ�����ж���
	for(; iIdx < numVertexs; iIdx++)
	{
		//ѭ��ĳ�������µ����б�;
		pEdge = m_vertexTable[iIdx]->adj;
		while(pEdge != NULL)
		{
			extractOverlappedEdge(pEdge);
			if(pEdge->m_leftSameEdges == 0) //˵��ȫ������ȡ��
			{
				delEdge(pEdge); //ɾ��֮��
			}
			//ָ�������ƶ�;
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



//��ָ������ȡ�ص��߹��ɵĻ�·��loop links);
//��ȡ�Ļ�·�����CArcLinkArray; ���ö����ʼ��;
bool
CGraphEnts::extractOverlappedEdge(IN CEdge* pEdge)
{
	//CEdge* pEdge;
	int edgeDepth = 0;	
	CEdge* pEdgeTemp = pEdge;
	edgeDepth = pEdge->m_leftSameEdges;
	if(edgeDepth > 1)  //�˱��ص�������1�����轫�ص���ÿ����һ��ȡ������pEdgeLinks��
	{
		int i = 0;
		i = edgeDepth % 2;   //������ż;
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
			pEdge->m_leftSameEdges -= 2;   //�˱�ʣ������ÿ�μ�ȥ2����������0����1��
		}
		return true;
	}
	return false;
}



//����������С��λ��������ȡ���ֵ;
//ͨ��ads_nameʵ�弯�ϣ��ж�ͼ��������С������ȷ�������꾫�ȣ�
//1.������С��λ; 2.�����������pointEqual��-ʵ���ò��ϣ�
void 
CGraphEnts::prsDotNumAndTol()
{
	CGetPointEqualAcGeTol objTol;
	AcGeTol equalPointTol;
	equalPointTol = objTol.getPointTolerance(m_sel,m_nDotNum);
}


//���ݶ�����ţ���ȡ��ȫ���ڽӶ��㣬�ж��Ƿ��γ�·����
//�γ�·����׼��1���ڽӵ�ΪҶ�ӣ� 2���ڽӵ㹹�ɻ��������ʹ�����
//����γ�·�������룻
void
CGraphEnts::testIfLoopONextVtxNode(const int v1) //,CArcLink& objCArcLink)
{
	CEdge* pEdge = NULL;
	pEdge = m_stackEdges.back(); //ȡβ��Ԫ�أ���������

	CEdge *pEdgeFind = NULL;
	int v2 = GetFirstNeighbor(v1,pEdgeFind);
	while(v2 != -1)
	{
		if(visited[v2] == 0) //û�б����ʹ���
		{
			if(getDegreeOfVertex(v2) == 1) //Ҷ�ӣ����룻
			{
				m_stackEdges.push_back(pEdgeFind);
				m_allLoops.push_back(m_stackEdges);
				//������֮��,��һ�£�
				m_stackEdges.pop_edge();
			}
		}
		else  //���ʹ��ģ��ж��Ƿ�·��
		{
			if(pEdgeFind != pEdge)
			{
				m_stackEdges.push_back(pEdgeFind);
				m_allLoops.push_back(m_stackEdges);
				//������֮��,��һ�£�
				m_stackEdges.pop_edge();
			}
		}
		v2 = GetNextNeighbor(v1,v2,pEdgeFind);  //���������ڽӵ㡣
	}
}




//��ͼ��
//���û�ѡ��ͼ�Σ�Ȼ����ͼ��
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
			m_vctEdges.push_back(pedge);  //�����ռ�����;
			insertEdge(pedge);
		}
	}
}


//�������ж���;
void
CGraphEnts::chopEdgeLinks()
{
	//��鶥����
	//if(m_vertexTable.size() <= 0)
	//{
	//	return;
	//}
	for(int i = 0; i < numVertexs; i++)
	{
		chopVertex(i);
	}
}


//��ĳ����������жϣ�����������2������;
//ÿ������ֻ�������Ϊ2;
//��ַ�����
//1.û��2���߲��һ�Σ�
//2.ÿ���һ�Σ�ʣ���������뼴ʱ���¶��㣬ÿ���ߵ��ص���ҲҪ��ʱ����;
void 
CGraphEnts::chopVertex(IN const int iIndex)
{
	int idxEdge = iIndex;
	int nCal = 0; //�����ã���Ϊż��ʱ�����һ����ͬ����;   
	CEdge* ptrNext = m_vertexTable[iIndex]->adj; //��һ����;  
	CEdge* ptrEdge = NULL;
	while(ptrNext != NULL)
	{
		for(int i = 0; ptrNext != NULL && i < 2; i++)  //һ�α���2����;  
		{
			if(idxEdge == ptrNext->index1)
			{
				ptrEdge = ptrNext; //ָ���;
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
		else  //��ָ��ָ��null;
		{
			if(idxEdge == ptrEdge->index1)
			{
				ptrEdge->path1 = NULL; //ָ���;
			}
			else
			{
				ptrEdge->path2 = NULL;
			}

			//ptrEdge = NULL;
		}

		//�����¶���; ���ԭ������;
		int iNewIndex = insertSameVertex(iIndex);
		m_vertexTable[iNewIndex]->adj = ptrNext;
		//�Ѹö������������ж�����Ŷ����³��¶���;
		CEdge* pEdge = ptrNext;
		while(pEdge != NULL)
		{
			//�Ժ���ÿ���ߵ��ص��ߵ����ҲҪ����;
			//���ﲻ�ܸ���pEdgeTmp,����һ����;
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

			//���������ߵ���Ӧ������Ÿ���;
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
		idxEdge = iNewIndex;  //���±����;	
	}
}



//���ݵ�ǰ���������¶��㣨�϶����ظ����㣩
//����ֵ���������;
int
CGraphEnts::insertSameVertex(IN const int vtxIndex)
{
	//int index;
	//index = findVertexNode(pt);
	AcGePoint3d pt;
	pt = m_vertexTable[vtxIndex]->pt;
	if(vtxIndex >= 0)  //����Ϸ���
	{
		CVertex* pobjVertex = new CVertex(pt);  //��������
		m_vertexTable.push_back(pobjVertex);
		visited.push_back(0); //��֮������ʱ��;

		numVertexs++;
		int index = numVertexs -1;  //��Ŵ�0��ʼ��	
		return index; 
	}
	else  //�ҵ��˸õ�����ڶ�����С��򲻲���õ㣬����-1��һ�㲻����ִ������;
	{
		return -1;
	}
}


//�ж�2�����Ƿ����;
//�������޸�;
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


//��ȱ���ͼ�󣬵õ�·�����ϣ�
//Ȼ�󣬶�·��������ʵ�ı�ʵ�壬�������ʵ·������Ϊ����·��֮������ظ�ʹ��ĳЩ�ߣ�
//������ʵ·�������ص�����
void 
CGraphEnts::to_polyline(IN ads_name ssUsr) 
{
	CTimeElapse objTimeElpsed; //ͳ��ʱ��; 	

	ads_name_set(ssUsr,m_sel);  //ѡ��  

	//����������������ֵ��С��λ��;
	if(ads_name_nil(ssUsr))
	{
		acutPrintf(_T("\n user select pickset is empty."));
		return;	
	}
	else
	{
		prsDotNumAndTol();
	}

	createGraph();  //��ͼ;

	CTimeElapse objTimeElpsedAnother;

	//��ȡ�ص���
	extractOverlappedEdges();	

	//DFS3(); //��ӡͼ���������

	chopEdgeLinks(); //(������ȴ���3�Ķ�����֮��ʹ�ó����С��3��

	//DFS3();  //��ӡͼ���������;

	//��0��ʼ��������ȡ����·��;
	resetvisited(); //������һ�·��ʱ�ʶ;
	for(int i = 0; i < numVertexs; i++)
	{
		extractEdgeLinks(i);
	}
	//���ԣ���ӡ����ȡ�õ�·��;
	

	//��ӡ��ȡ��·�����������˺�ɾ��;
	m_allLoops.testPrintOriginalPath();

	m_allLoops.correctByRealEdges();

	////����������ɶ����ߵ�·�����Ѿ������õ�·����;
	m_allLoops.testm_edgeLinkArrayToPolyline();

	m_allLoops.toPolylines();
}

//=============����Ϊͼ��========================================end======




//==========================
//class  CGetAcGeTol
//���룺ͼ�μ���
//�������
//==========================
CGetPointEqualAcGeTol::CGetPointEqualAcGeTol()
{
	m_tol = 0.000001;  //Ĭ��ֵ;
}

CGetPointEqualAcGeTol::~CGetPointEqualAcGeTol()
{
}


//����ͼ��ʵ�弯�ϵ�������߼�����ֵ����������ֵ�ķ�Χ���ʵ�����
//���ֵ�������
//1.0Ĭ����0.000001��
//1.1����������ֵ����1000������Ϊ0.00001;
//1.2����������ֵС��10������Ϊ0.0000001; ���ϡ�
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
	//�жϵ�������ֵ����;
	double xRT = ptRightTop.x;
	double yRT = ptRightTop.y;
	double xLB = ptLeftBot.x;
	double yLB = ptLeftBot.y;

	xRT = abs(xRT);
	yRT = abs(yRT);
	xLB = abs(xLB);
	yLB = abs(yLB);

	//������;
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
//����topln
//=====================================
void 
to_polylines(void)
{
	//�û�ѡ��ʵ�弯��;
	ads_name ssUsr;
	CSelectEnts objSelect;
	objSelect.usrSelect(ssUsr);

	CGraphEnts obj;
	obj.to_polyline(ssUsr); 

	//release pickset
	acedSSFree(ssUsr);   
	// Add your code for command ZHGpc2010.owal2016topolyline here
}