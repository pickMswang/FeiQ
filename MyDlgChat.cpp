// MyDlgChat.cpp : 实现文件
//

#include "stdafx.h"
#include "0809Feiq.h"
#include "MyDlgChat.h"
#include "afxdialogex.h"
#include "0809FeiqDlg.h"

// CMyDlgChat 对话框

IMPLEMENT_DYNAMIC(CMyDlgChat, CDialogEx)

CMyDlgChat::CMyDlgChat(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyDlgChat::IDD, pParent)
	, m_edtContent(_T(""))
{

}

CMyDlgChat::~CMyDlgChat()
{
}

void CMyDlgChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_edtContent);
	DDX_Control(pDX, IDC_LIST1, m_lstContent);
}


BEGIN_MESSAGE_MAP(CMyDlgChat, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDlgChat::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CMyDlgChat::OnBnClickedCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMyDlgChat 消息处理程序


void CMyDlgChat::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	STRU_DATAINFO sd;
	sd.m_ntype = _DEF_PROTOCOL_DATAINFO;
	strcpy_s(sd.m_szBuffer,sizeof(sd.m_szBuffer),m_edtContent);
	sd.m_nLen = m_edtContent.GetLength() + 1;

	CString strip;
	GetWindowText(strip);
	
	
	if(((CMy0809FeiqDlg*)GetParent())->GetUDPMediator()->SendData(inet_addr(strip),(char*)&sd,sizeof(sd)))
	{
		m_lstContent.AddString("I say:" +m_edtContent );
		m_edtContent = "";
		UpdateData(FALSE);
	}
}


void CMyDlgChat::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
	DestroyWindow();
}


void CMyDlgChat::OnDestroy()
{
	CDialogEx::OnDestroy();

	CString strip;
	GetWindowText(strip);
	//将自己从map中移除
	GetParent()->SendMessage(UM_DESTROYMSG,(WPARAM)&strip,0);
	// TODO: 在此处添加消息处理程序代码
}
