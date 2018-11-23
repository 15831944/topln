// findDist.cpp : 实现文件
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


// findDist 对话框

IMPLEMENT_DYNAMIC(findDistDlg, CDialog)

findDistDlg::findDistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(findDistDlg::IDD, pParent), minDistByUserInput(0)
{
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


// findDist 消息处理程序 

void findDistDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void findDistDlg::OnBnClickedButtonFirst()    
{
	// TODO: 在此添加控件通知处理程序代码   
	AfxMessageBox(_T("first button"),0,0);  
}

//运行最小距离检查;
void findDistDlg::OnBnClickedButtonRun()
{
	// TODO: 在此添加控件通知处理程序代码   
	//检查用户输入的dist值是否合法   
	UpdateData(TRUE);

	AcGeTol objtol;
	double dbltmp = objtol.equalPoint();    
	if(minDistByUserInput <= dbltmp)      
	{
		acutPrintf(_T("\nuser's input is %f"),minDistByUserInput);      
		AfxMessageBox(_T("\n用户输入的距离值太小了..."),0,0);       
		return;
	}

	int nResl = -1;
	this->EndDialog(nResl);

	acutPrintf(_T("\nuser's input is %f"),minDistByUserInput);   

	
	//查找点对;
	//选择实体集; 
	ads_name ss;
	CSelectEnts objSel;
	acdbNameClear(ss); //设置为nil;  
	objSel.usrSelect(ss); 

	double dblMinDist;
	vector<pair<void*,void*>> vAllPointPair;     
	CFindGapBtwPoints objFindPointPairs;      
	objFindPointPairs.inputMinDistByUser(minDistByUserInput); 
	objFindPointPairs.inputAdsName(ss);   
	objFindPointPairs.getPointPair(vAllPointPair);    

	//点对过滤;
	//不过滤算了; 只有一种情况要过滤掉：点对属于同一条直线段;     

	//打印点对;
	int nlen = 0;
	nlen = vAllPointPair.size();
	pair<void*,void*> vpairTmp;
	vector<pair<void*,void*>>::iterator itrPair = vAllPointPair.begin();  
	for(; itrPair != vAllPointPair.end(); itrPair++)  
	{
		vpairTmp = (pair<void*,void*>)(*itrPair); 
		((SAttachData*)(vpairTmp.first))->print(); 
		((SAttachData*)(vpairTmp.second))->print(); 
	}
	//运行后让按钮enable;
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
	;
}


//===================
//调用findDist窗口
//
//===================
void
mendGapBtwPointPairDlg()
{
	//窗口
	CAcModuleResourceOverride resOverride;
	findDistDlg myDialog;
	myDialog.DoModal();
};
