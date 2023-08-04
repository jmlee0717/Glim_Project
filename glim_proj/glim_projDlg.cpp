
// glim_projDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "glim_proj.h"
#include "glim_projDlg.h"
#include "afxdialogex.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CglimprojDlg 대화 상자



CglimprojDlg::CglimprojDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIM_PROJ_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CglimprojDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUND_STATIC, m_GroundStatic);
}

BEGIN_MESSAGE_MAP(CglimprojDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DRAW_BTN, &CglimprojDlg::OnBnClickedDrawBtn)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CglimprojDlg 메시지 처리기

BOOL CglimprojDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CglimprojDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CglimprojDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 원을 그리기 위한 플래그 setting을 확인하고 조건에 맞으면 원을 그림
		if(m_bDrawFlag)
			drawData(&dc);
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CglimprojDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CglimprojDlg::OnBnClickedDrawBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	CString strRadius;
	GetDlgItem(IDC_RADIUS_EDIT)->GetWindowTextW(strRadius);

	if (strRadius.IsEmpty())
	{
		AfxMessageBox(L"원의 반지름을 입력하세요");
		return;
	}

	std::wstring wstrRadius = strRadius;
	m_nRadius = std::stoi(wstrRadius);

	// 원이 그려질 CStatic 영역의 좌표를 구함
	CRect rectClient;
	m_GroundStatic.GetClientRect(rectClient);

	// 랜덤한 위치를 구하여 x, y 좌표를 구한다. -> rectClient 영역안에 x,y 좌표가 설정 되도록 처리
	//1) (rectClient.right - 2 * m_nRadius): 객체 반경의 두 배(지금).이렇게 하면 개체의 중심이 클라이언트 영역의 오른쪽 가장자리에서 m_nRadius 거리 이상 떨어져 있음.
	//2) rand() % (rectClient.right - 2 * m_nRadius) : [0, rectClient.right - 2 * m_nRadius) 범위의 임의 값을 생성.
	//3) + m_nRadius: m_nRadius를 무작위 랜덤 값에 추가하여 중심이 클라이언트 영역의 왼쪽 가장자리에서 최소 m_nRadius 거리에 있도록 함
	m_nPointx = rand() % (rectClient.right -  2* m_nRadius) + m_nRadius;
	m_nPointy = rand() % (rectClient.bottom - 2 * m_nRadius) + m_nRadius;

	// 위의 적용한 내용을 OnPaint에서 실행하기 위한 플래그값 setting
	m_bDrawFlag = TRUE;
	Invalidate();

}

// 원을 그리고 중심좌표를 그리는 메인 함수
void CglimprojDlg::drawData(CDC* pDC)
{
	CRect rect;
	rect.SetRect(m_nPointx, m_nPointy, m_nPointx + m_nRadius, m_nPointy + m_nRadius);

	// 랜덤한 rect 영역의 중심 좌표를 구함.
	m_CenterPt.x = (rect.left + rect.right) / 2;
	m_CenterPt.y = (rect.top + rect.bottom) / 2;

	// 원이 그려질 CStatic 영역의 좌표를 구함
	CRect rectClient;
	// 전체 윈도우 기준으로 m_GroundStatic의 실제 영역 좌표
	m_GroundStatic.GetWindowRect(rectClient);
	// 현재 다이얼로그 기준으로 m_GroundStatic가 위치한 상대적인 영역 좌표
	ScreenToClient(rectClient);

	if (rectClient.PtInRect(m_CenterPt))
	{
		drawCircle(pDC, rect);
		drawCenter(pDC);
	}
	else
		displayText(pDC, _T("원의 중심 좌표가 클라이언트 영역을 벗어납니다"));
}

// 원을 그리고 색칠하는 메인 함수
void CglimprojDlg::drawCircle(CDC* pDC, CRect rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 3, RGB(0, 255, 255));
	CPen* pOldPen = pDC->SelectObject(&pen);

	CBrush brush(RGB(255, 255, 0)); // 초록색 브러시 생성
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	// 원안을 초록색으로 색칠하기
	pDC->Ellipse(m_CenterPt.x - m_nRadius / 2, m_CenterPt.y - m_nRadius / 2, m_CenterPt.x + m_nRadius / 2, m_CenterPt.y + m_nRadius / 2);

	// 랜덤한 위치를 구한 x, y 좌표에 반지름 만큼의 width 와 height를 할당한 rect 영역을 지정.
	pDC->Ellipse(rect);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

// 원의 중심을 표출하는 함수
void CglimprojDlg::drawCenter(CDC* pDC)
{
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0)); // 빨간색 펜 생성
	CPen* pOldPen = pDC->SelectObject(&pen);

	// 수직 선 그리기
	pDC->MoveTo(m_CenterPt.x, m_CenterPt.y);
	// 중심 좌표에서 위로 10 픽셀 만큼 선을 그림
	pDC->LineTo(m_CenterPt.x, m_CenterPt.y - 10);
	// 중심 좌표에서 아래로 10 픽셀 만큼 선을 그림
	pDC->LineTo(m_CenterPt.x, m_CenterPt.y + 10);

	// 수평 선 그리기
	pDC->MoveTo(m_CenterPt.x, m_CenterPt.y);
	// 중심 좌표에서 좌로 10 픽셀 만큼 선을 그림
	pDC->LineTo(m_CenterPt.x - 10, m_CenterPt.y);
	// 중심 좌표에서 우로 10 픽셀 만큼 선을 그림
	pDC->LineTo(m_CenterPt.x + 10, m_CenterPt.y);

	pDC->SelectObject(pOldPen);

	displayText(pDC, _T("원의 중심 좌표"));
}

// 실시간으로 현재 좌표에 대한 정보를 화면에 표출
void CglimprojDlg::displayText(CDC* pDC, CString strMsg)
{
	CRect rectClient;
	// 전체 윈도우 기준으로 m_GroundStatic의 실제 영역 좌표
	m_GroundStatic.GetWindowRect(rectClient);
	// 현재 다이얼로그 기준으로 m_GroundStatic가 위치한 상대적인 영역 좌표
	ScreenToClient(rectClient);
	CString strMessage;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->SetTextAlign(TA_LEFT);
	strMessage.Format(L"%s (x=%d, y=%d)", strMsg, m_CenterPt.x - rectClient.left, m_CenterPt.y - rectClient.top);
	pDC->TextOut(20, 20, strMessage);
}

//마우스를 클릭시 좌표를 기준으로 원을 그리는 함수
void CglimprojDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 원이 그려질 CStatic 영역의 좌표를 구함
	CRect rectClient;
	// 전체 윈도우 기준으로 m_GroundStatic의 실제 영역 좌표
	m_GroundStatic.GetWindowRect(rectClient);
	// 현재 다이얼로그 기준으로 m_GroundStatic가 위치한 상대적인 영역 좌표
	ScreenToClient(rectClient);

	CString strRadius;
	GetDlgItem(IDC_RADIUS_EDIT)->GetWindowTextW(strRadius);

	if (strRadius.IsEmpty())
	{
		AfxMessageBox(L"원의 반지름을 입력하세요");
		return;
	}

	std::wstring wstrRadius = strRadius;
	m_nRadius = std::stoi(wstrRadius);

	// 현재 다이얼로그 기준으로 m_GroundStatic가 위치한 상대적인 영역 좌표 내에 마우스 포인트가 있는지 체크
	if (rectClient.PtInRect(point))
	{
		m_nPointx = point.x - m_nRadius / 2;
		m_nPointy = point.y - m_nRadius / 2;

		m_bDrawFlag = TRUE;
		Invalidate();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}
