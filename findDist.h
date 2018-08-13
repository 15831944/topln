#pragma once

#include "resource.h"
#include "afxwin.h"


// findDist 对话框

class findDistDlg : public CDialog
{
	DECLARE_DYNAMIC(findDistDlg)

public:
	findDistDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~findDistDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	CEdit m_edit_maxDist;
	afx_msg void OnBnClickedButtonFirst();
};
