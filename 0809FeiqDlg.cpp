
// 0809FeiqDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "0809Feiq.h"
#include "0809FeiqDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy0809FeiqDlg 对话框


HHOOK  m_hKeyHook = NULL;
CMy0809FeiqDlg::CMy0809FeiqDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy0809FeiqDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pUDPMediator = new UDPMediator(this);
	
}

void CMy0809FeiqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstip);
}

BEGIN_MESSAGE_MAP(CMy0809FeiqDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_MESSAGE(UM_ONLINEMSG,&CMy0809FeiqDlg::OnLineMsg)
	ON_MESSAGE(UM_OFFLINEMSG,&CMy0809FeiqDlg::OffLineMsg)
	ON_MESSAGE(UM_DATAMSG,&CMy0809FeiqDlg::DataMsg)
	ON_MESSAGE(UM_DESTROYMSG,&CMy0809FeiqDlg::DestroyMsg)
	ON_WM_KEYDOWN()
	ON_LBN_DBLCLK(IDC_LIST1, &CMy0809FeiqDlg::OnLbnDblclkList1)
END_MESSAGE_MAP()


// CMy0809FeiqDlg 消息处理程序
LRESULT CMy0809FeiqDlg::OnLineMsg(WPARAM wparam,LPARAM lparam)
{
	in_addr addr;
	addr.S_un.S_addr = lparam;
	CString str;
	str = inet_ntoa(addr);
	m_lstip.AddString(str);
	return 0;
}

LRESULT CMy0809FeiqDlg::OffLineMsg(WPARAM wparam,LPARAM lparam)
{
	in_addr addr;
	addr.S_un.S_addr = lparam;
	CString strFriend;
	strFriend = inet_ntoa(addr);
	CString strip;
	for(int i = 0;i <m_lstip.GetCount();i++)
	{
		m_lstip.GetText(i,strip);
		if(strip == strFriend)
		{
			m_lstip.DeleteString(i);
			break;
		}
	}
	return 0;
}

LRESULT CMy0809FeiqDlg::DataMsg(WPARAM wparam,LPARAM lparam)
{
	//找到lparam IP 对应的窗口
	char *szbuf = (char*)wparam;
	in_addr addr ;
	addr.S_un.S_addr = lparam;
	CString strbuf = inet_ntoa(addr);
	
	CMyDlgChat *pDlg = GetDlg(inet_ntoa(addr));
	//显示
	pDlg->m_lstContent.AddString(strbuf+ "say:"+szbuf);
	return 0;
}

LRESULT CMy0809FeiqDlg::DestroyMsg(WPARAM wparam,LPARAM lparam)
{
	CString strip = *(CString *)wparam;
	auto ite = m_mapIpToDlg.find(strip);
	if(ite != m_mapIpToDlg.end())
	{
		delete ite->second;
		ite->second = nullptr;
		m_mapIpToDlg.erase(ite);
	}
	return 0;
}


LRESULT CALLBACK KeyboardProc(  int code, WPARAM wParam, LPARAM lParam)
{
	if(wParam == 0X0D)
	{
		return 1;
	}

	return CallNextHookEx(m_hKeyHook,code,wParam,lParam);
}



BOOL CMy0809FeiqDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if(!m_pUDPMediator->open())
	{
		MessageBox(_T("open fail"));
		return FALSE;

	}
	//初始化钩子
	m_hKeyHook = SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,NULL,GetCurrentThreadId());
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}




void CMy0809FeiqDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy0809FeiqDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy0809FeiqDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy0809FeiqDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if(m_pUDPMediator)
	{
		m_pUDPMediator->close();
		delete m_pUDPMediator;
		m_pUDPMediator = nullptr;
	}

	if(m_hKeyHook)
	{
		UnhookWindowsHookEx(m_hKeyHook);
		m_hKeyHook =  NULL;
	}

	auto ite = m_mapIpToDlg.begin();
	while(ite != m_mapIpToDlg.end())
	{
		delete ite->second;
		ite->second = nullptr;
		ite++;
	}

	m_mapIpToDlg.clear();
}


void CMy0809FeiqDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CMy0809FeiqDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	/*if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)
	{
		return FALSE;
	}*/
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMy0809FeiqDlg::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	//获得当前点击项
	int nindex = m_lstip.GetCurSel();
	if(-1== nindex)return;
	//获得选中的ip
	CString strip;
	m_lstip.GetText(nindex,strip);
	//判断当前好友有么有聊天窗口，如果没有，则窗口，否则，直接显示
	GetDlg(strip);
	
}
CMyDlgChat *CMy0809FeiqDlg::GetDlg(CString strip)
{
	CMyDlgChat *pdlg =  m_mapIpToDlg[strip];
	if(!pdlg)
	{
		pdlg = new CMyDlgChat;
		pdlg->Create(IDD_DLG_CHAT);
		m_mapIpToDlg[strip] = pdlg;
	}
	
    pdlg->ShowWindow(SW_SHOW);
	pdlg->SetWindowText(strip);

	return pdlg;
	
}
