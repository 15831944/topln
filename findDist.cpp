// findDist.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "findDist.h"
#include "resource1.h"


// findDist �Ի���

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


// findDist ��Ϣ������� 

void findDistDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void findDistDlg::OnBnClickedButtonFirst() 
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������  
	AfxMessageBox(_T("first button"),0,0);  
}
