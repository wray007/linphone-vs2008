
// MicroVoiceLiteDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CMicroVoiceLiteDlg 对话框
class CMicroVoiceLiteDlg : public CDialog
{
// 构造
public:
	CMicroVoiceLiteDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MICROVOICELITE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBntMakecall();
	afx_msg void OnBnClickedBntRegister();
	afx_msg void OnBnClickedBntDropcall();
	afx_msg void OnBnClickedBntUnregister();
	CEdit mUsername;
	CEdit mPassword;
	CEdit mHost;
	CEdit mDialNum;
	afx_msg void OnBnClickedCancel();
	CStatic mStatus;
	CButton mHold;
	afx_msg void OnBnClickedCheckHold();
	CButton mAGC;
	CButton mNR;
	afx_msg void OnBnClickedCheckAgc();
	afx_msg void OnBnClickedCheckNr();
	CButton mRecord;
	afx_msg void OnBnClickedCheckRecord();
	CButton mAEC;
	afx_msg void OnBnClickedCheckAec();
	CEdit mDTMF;
	afx_msg void OnBnClickedBntSendDtmf();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonDtmf1();
	afx_msg void OnBnClickedButtonDtmf2();
	afx_msg void OnBnClickedButtonDtmf3();
	afx_msg void OnBnClickedButtonDtmf4();
	afx_msg void OnBnClickedButtonDtmf5();
	afx_msg void OnBnClickedButtonDtmf6();
	afx_msg void OnBnClickedButtonDtmf7();
	afx_msg void OnBnClickedButtonDtmf8();
	afx_msg void OnBnClickedButtonDtmf9();
	afx_msg void OnBnClickedButtonDtmf10();
	afx_msg void OnBnClickedButtonDtmf0();
	afx_msg void OnBnClickedButtonDtmf11();
	CComboBox mCamera;
	afx_msg void OnCbnSelchangeComboCamList();
	CEdit mVideoWnd;
};
