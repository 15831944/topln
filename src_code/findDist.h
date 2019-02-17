#pragma once

#include "..\resource.h"
#include "afxwin.h"
#include "findTheGapBtwPoints.h"
#include "pointMap.h"

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
	afx_msg void OnBnClickedButtonRun();  
private:
	// 用户用户输入的最小距离值
	double minDistByUserInput;
	ads_name m_ss;  //用户选择的实体集;
	vector<pair<void*,void*>>  m_vPointPairs; //查出来的小于某个距离的点对集合;	  
	CFindGapBtwPoints m_objFindMinGap; //查找符合距离要求的点对;    
	COptOfPointPairs m_optOfPtPair;
//public:
//	afx_msg void OnBnClickedButtonRun();  


	//models defined by users;  
private:
	void getInputGapVal();  
public:
	afx_msg void OnBnClickedButtonNext();
}; 



