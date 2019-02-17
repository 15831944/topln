#pragma once

#include "..\resource.h"
#include "afxwin.h"
#include "findTheGapBtwPoints.h"
#include "pointMap.h"

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
	afx_msg void OnBnClickedButtonRun();  
private:
	// �û��û��������С����ֵ
	double minDistByUserInput;
	ads_name m_ss;  //�û�ѡ���ʵ�弯;
	vector<pair<void*,void*>>  m_vPointPairs; //�������С��ĳ������ĵ�Լ���;	  
	CFindGapBtwPoints m_objFindMinGap; //���ҷ��Ͼ���Ҫ��ĵ��;    
	COptOfPointPairs m_optOfPtPair;
//public:
//	afx_msg void OnBnClickedButtonRun();  


	//models defined by users;  
private:
	void getInputGapVal();  
public:
	afx_msg void OnBnClickedButtonNext();
}; 



