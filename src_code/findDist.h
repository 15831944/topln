#pragma once

#include "..\resource.h"
#include "afxwin.h"
#include "findTheGapBtwPoints.h"

//调用对话框;
void mendGapBtwPointPairDlg();

// findDist 对话框

class findDistDlg : public CDialog
{
	DECLARE_DYNAMIC(findDistDlg)  

public:
	findDistDlg(CWnd* pParent = NULL);   // 标准构造函数; 
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
private:
	// 用户用户输入的最小距离值
	double minDistByUserInput;
	CFindGapBtwPoints m_objFindMinGap; //查找符合距离要求的点对;  
public:
	afx_msg void OnBnClickedButtonRun();  
}; 



