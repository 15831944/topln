// findDist.cpp : ʵ���ļ�
//
#include "..\stdafx.h"

//#include <aced.h>
//#include <rxregsvc.h>
//#include <adscodes.h>
//#include <dbsymtb.h>
//#include <dblayout.h>
//#include <dbapserv.h>
//#include <acdblmgr.h>
//#include <dbents.h>
//#include <dbpl.h>
//#include <dbid.h>
//#include <acutads.h>
//#include <string.h>
//#include <stdio.h>
//#include <acedads.h>
//#include <dbmain.h>
//#include <geplin2d.h>  
//#include <math.h>
//#include <acarray.h>
//#include <dbdict.h>
//#include <dbgroup.h>
//#include <rxobject.h>
//#include <gelnsg2d.h>
//#include <gevec2d.h>

#include <string>
#include <iostream> 



#include "findDist.h"
#include "..\resource1.h"


// findDist �Ի���

IMPLEMENT_DYNAMIC(findDistDlg, CDialog)

findDistDlg::findDistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(findDistDlg::IDD, pParent), minDistByUserInput(0)
{
	m_vPointPairs.clear();  //empty it;
	//ѡ��ʵ�弯;
	CSelectEnts objSel;
	acdbNameClear(m_ss); //����Ϊnil;  
	objSel.usrSelect(m_ss); 
}

findDistDlg::~findDistDlg()
{	
}

void findDistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Max_dist, m_edit_maxDist);
	DDX_Text(pDX, IDC_EDIT_Max_dist, minDistByUserInput);
}


BEGIN_MESSAGE_MAP(findDistDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &findDistDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_First, &findDistDlg::OnBnClickedButtonFirst)   
	ON_BN_CLICKED(IDC_BUTTON_RUN, &findDistDlg::OnBnClickedButtonRun)  
END_MESSAGE_MAP()


// findDist ��Ϣ������� 

void findDistDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void findDistDlg::OnBnClickedButtonFirst()    
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������   
	//AfxMessageBox(_T("first button"),0,0);  
	bool bflag = false;
	AcGePoint3d pt0;
	AcGePoint3d pt1;
	bflag = m_optOfPtPair.first(pt0,pt1);

	//�Ŵ��һ�����;
}

//������С������;
void findDistDlg::OnBnClickedButtonRun()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������   
	//����û������distֵ�Ƿ�Ϸ�   
	UpdateData(TRUE);   

	AcGeTol objtol;
	double dbltmp = objtol.equalPoint();    
	if(minDistByUserInput <= dbltmp)      
	{
		acutPrintf(_T("\nuser's input is %f"),minDistByUserInput);      
		AfxMessageBox(_T("\n�û�����ľ���ֵ̫С��..."),0,0);       
		//return;
	}

	//int nResl = -1;
	//this->EndDialog(nResl);

	acutPrintf(_T("\nuser's input is %f"),minDistByUserInput);   

	
	//���ҵ��;
	//check the m_ss
	long nlen = 0;
	acedSSLength(m_ss,&nlen);
	if(nlen <= 0)
	{
		return;
	}


	double dblMinDist;
	//vector<pair<void*,void*>> vAllPointPair;     
	CFindGapBtwPoints objFindPointPairs;      
	objFindPointPairs.inputMinDistByUser(minDistByUserInput); 
	objFindPointPairs.inputAdsName(m_ss);   
	objFindPointPairs.getPointPair(m_vPointPairs);   

	//������Ƿ�Ϊ��
	if(m_vPointPairs.size() > 0)
	{
		m_optOfPtPair.inputPointPairs(&m_vPointPairs);
	}

	//��Թ���;
	//����������; ֻ��һ�����Ҫ���˵����������ͬһ��ֱ�߶�;     

	//��ӡ���;
	//int nlen = 0;
	//nlen = vAllPointPair.size();
	//pair<void*,void*> vpairTmp;
	//vector<pair<void*,void*>>::iterator itrPair = vAllPointPair.begin();  
	//acutPrintf(_T("\nOnBnClickedButtonRun--------"));
	//for(; itrPair != vAllPointPair.end(); itrPair++)  
	//{
	//	vpairTmp = (pair<void*,void*>)(*itrPair); 
	//	((SAttachData*)(vpairTmp.first))->print(); 
	//	((SAttachData*)(vpairTmp.second))->print(); 
	//}
	//acutPrintf(_T("\n--------OnBnClickedButtonRun")); 
	//���к��ð�ťenable;
	/*CWnd* pWnd_help = GetDlgItem(IDC_BUTTON_Help);
	CWnd* pWnd_btn_first = GetDlgItem(IDC_BUTTON_First);   
	CWnd* pWnd_btn_next = GetDlgItem(IDC_BUTTON_NEXT);
	CWnd* pWnd_btn_conn = GetDlgItem(IDC_BUTTON_Connect);
	CWnd* pWnd_btn_conn_next = GetDlgItem(IDC_BUTTON_CONNECT_Next);  
	CWnd* pWnd_btn_conn_all  = GetDlgItem(IDC_BUTTON_Connect_all);    
	pWnd_help->EnableWindow(true);
	pWnd_btn_first->EnableWindow(true);
	pWnd_btn_next->EnableWindow(true);
	pWnd_btn_conn->EnableWindow(true);
	pWnd_btn_conn_next->EnableWindow(true);
	pWnd_btn_conn_all->EnableWindow(true);*/
}


void
findDistDlg::getInputGapVal()
{
	CWnd* pWndDistGap = GetDlgItem(IDC_EDIT_Max_dist);
}


//===================
//����findDist����
//
//===================
void
mendGapBtwPointPairDlg()
{
	//����
	CAcModuleResourceOverride resOverride;
	findDistDlg myDialog;
	myDialog.DoModal();
};
