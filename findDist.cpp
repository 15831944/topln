// findDist.cpp : 实现文件
//

#include "stdafx.h"
#include "findDist.h"
#include "resource.h"


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
}


BEGIN_MESSAGE_MAP(findDistDlg, CDialog)
END_MESSAGE_MAP()


// findDist 消息处理程序
