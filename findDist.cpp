// findDist.cpp : 实现文件
//

#include "stdafx.h"
#include "findDist.h"
#include "resource1.h"


// findDist 对话框

IMPLEMENT_DYNAMIC(findDistDlg, CDialog)

findDistDlg::findDistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(findDistDlg::IDD, pParent)
{

}

findDistDlg::~findDistDlg()
{
}

void findDistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Max_dist, m_edit_maxDist);
}


BEGIN_MESSAGE_MAP(findDistDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &findDistDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_First, &findDistDlg::OnBnClickedButtonFirst)
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
