#pragma once

#include "..\resource.h"
#include "afxwin.h"
#include "findTheGapBtwPoints.h"

//���öԻ���;
void mendGapBtwPointPairDlg();

// findDist �Ի���

class findDistDlg : public CDialog
{
	DECLARE_DYNAMIC(findDistDlg)  

public:
	findDistDlg(CWnd* pParent = NULL);   // ��׼���캯��; 
	virtual ~findDistDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };   

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();  
	CEdit m_edit_maxDist;
	afx_msg void OnBnClickedButtonFirst();    
private:
	// �û��û��������С����ֵ
	double minDistByUserInput;
	CFindGapBtwPoints m_objFindMinGap; //���ҷ��Ͼ���Ҫ��ĵ��;  
public:
	afx_msg void OnBnClickedButtonRun();  
}; 



