
// 0809FeiqDlg.h : 头文件
//

#pragma once
#include "UDPMediator.h"
#include "afxwin.h"
#include "MyDlgChat.h"
#include <list>
#include <map>

using namespace std;
// CMy0809FeiqDlg 对话框
class CMy0809FeiqDlg : public CDialogEx
{
// 构造
public:
	CMy0809FeiqDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY0809FEIQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLbnDblclkList1();
	LRESULT OnLineMsg(WPARAM,LPARAM);
	LRESULT OffLineMsg(WPARAM,LPARAM);
	LRESULT DataMsg(WPARAM,LPARAM);
	LRESULT DestroyMsg(WPARAM,LPARAM);
	CMyDlgChat *GetDlg(CString strip);
	DECLARE_MESSAGE_MAP()
private:
	IMediator *m_pUDPMediator;
	//list<CMyDlgChat*> m_lstDlg;
	map<CString ,CMyDlgChat*> m_mapIpToDlg;
	CListBox m_lstip;
public:
	IMediator * GetUDPMediator()
	{
		return m_pUDPMediator;
	}
	
};
