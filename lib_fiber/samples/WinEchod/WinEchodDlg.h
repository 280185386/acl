
// WinEchodDlg.h : ͷ�ļ�
//

#pragma once


// CWinEchodDlg �Ի���
class CWinEchodDlg : public CDialogEx
{
// ����
public:
	CWinEchodDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CWinEchodDlg();

// �Ի�������
	enum { IDD = IDD_WINECHOD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAbort();
	afx_msg void OnBnClickedOpenDos();
private:
	FILE* m_dosFp;
};
