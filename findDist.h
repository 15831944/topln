#pragma once

#include "resource.h"


// findDist �Ի���

class findDistDlg : public CDialog
{
	DECLARE_DYNAMIC(findDistDlg)

public:
	findDistDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~findDistDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
