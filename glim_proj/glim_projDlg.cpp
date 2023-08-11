
// glim_projDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "glim_proj.h"
#include "glim_projDlg.h"
#include "afxdialogex.h"

#include <string>

#include <iostream>
using namespace std;

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
	ON_WM_TIMER()
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
	// 원이 그려질 CStatic 영역의 배경을 처리
	// OnPaint의 디바이스 컨텍스트가 먼저 초기화 되어야 하기 때문에 Timer로 처리
	// 호출 순서 OnInitDialog -> OnPaint -> OnTimer
	SetTimer(1, 100, NULL);

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


	CString strSize;
	GetDlgItem(IDC_SIZE_EDIT)->GetWindowTextW(strSize);

	if (strSize.IsEmpty())
	{
		AfxMessageBox(L"원의 지름을 입력하세요");
		return;
	}

	std::wstring wstrSize = strSize;
	m_nSize = std::stoi(wstrSize);

	// 원이 그려질 CStatic 영역의 좌표를 구함
	CRect rectClient;
	m_GroundStatic.GetClientRect(rectClient);

	// rectClient 영역안에 x,y 좌표가 설정 되도록 처리
	int maxX = rectClient.right - m_nSize;		// 영역의 가로 좌표의 최대 길이에서 원의 지름을 뺀 최대 길이를 랜덤 x 범위로 함
	int maxY = rectClient.bottom - m_nSize;     // 영역의 세로 좌표의 최대 길이에서 원의 지름을 뺀 최대 길이를 랜덤 y 범위로 함

	// 랜덤한 위치를 구하여 x, y 좌표를 구한다. 
	m_nPointx = rand() % maxX;
	m_nPointy = rand() % maxY;


	// 위의 적용한 내용을 OnPaint에서 실행하기 위한 플래그값 setting
	m_bDrawFlag = TRUE;
	Invalidate();

}

// 원을 그리기 위한 rect의 Start 좌표를 랜덤으로 구하고 클라이언트 영역을 벗어나면 예외처리하는 함수
void CglimprojDlg::drawData(CDC* pDC)
{
	// 원이 그려질 CStatic 영역의 좌표를 구함
	CRect rectClient;
	// 전체 윈도우 기준으로 m_GroundStatic의 실제 영역 좌표
	m_GroundStatic.GetWindowRect(rectClient);
	// 현재 다이얼로그 기준으로 m_GroundStatic가 위치한 상대적인 영역 좌표
	ScreenToClient(rectClient);

	static int nSttX = 0;
	static int nSttY = 0;
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();

	CClientDC dc(this);
	m_image.Draw(dc, rectClient.left, rectClient.top);

	CString strSize;
	GetDlgItem(IDC_SIZE_EDIT)->GetWindowTextW(strSize);

	if (strSize.IsEmpty())
	{
		AfxMessageBox(L"원의 지름을 입력하세요");
		return;
	}

	wstring wstrSize = strSize;
	int nRadius = stoi(wstrSize);

	// 원이 그려질 CStatic 영역의 좌표를 구함

	// rectClient 영역안에 x,y 좌표가 설정 되도록 처리
	int maxX = m_image.GetWidth() - nRadius;		// 영역의 가로 좌표의 최대 길이에서 원의 지름을 뺀 최대 길이를 랜덤 x 범위로 함
	int maxY = m_image.GetHeight() - nRadius;     // 영역의 세로 좌표의 최대 길이에서 원의 지름을 뺀 최대 길이를 랜덤 y 범위로 함

	// 랜덤한 위치를 구하여 x, y 좌표를 구한다. 
	nSttX = rand() % maxX;
	nSttY = rand() % maxY;

	// 원이 그려질 영역의 좌표를 구함
	CRect rect;
	rect.left = nSttX;
	rect.top = nSttY;
	rect.right = nSttX + nRadius * 2;
	rect.bottom = nSttY + nRadius * 2;

	if (rectClient.left <= rect.left && rectClient.right >= rect.right && rectClient.top <= rect.top && rectClient.bottom >= rect.bottom)
	{
		unsigned char* fm = (unsigned char*)m_image.GetBits();
		drawCircle(fm, nSttX, nSttY, nRadius, 0xff);
	}
	else
	{
		cout << "그러질 원의 영역이 Client 영역을 벗어남" << endl;

		AfxMessageBox(L"그러질 원의 영역이 Client 영역을 벗어남");
	}

}

// 원의 무게 중심 처리 및 원을 그리는 함수
void CglimprojDlg::drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;
	int nPitch = m_image.GetPitch();

	int nSumX = 0;
	int nSumY = 0;
	int nCount = 0;
	for (int j = y; j < y + nRadius * 2; j++) {
		for (int i = x; i < x + nRadius * 2; i++) {
			if (isInCircle(i, j, nCenterX, nCenterY, nRadius))
			{
				nSumX += i;
				nSumY += j;
				nCount++;
			}
		}
	}

	// 원의 무게 중심 좌표를 구함
	m_dCenterX = (double)nSumX / nCount;
	m_dCenterY = (double)nSumY / nCount;

	// 원이 그려질 CStatic 영역의 좌표를 구함
	CRect rectClient;
	rectClient.left = x;
	rectClient.top = y;
	rectClient.right = x + nRadius * 2;
	rectClient.bottom = y + nRadius * 2;

	CClientDC dc(this);

	CPen pen;
	pen.CreatePen(PS_SOLID, 5, RGB(0, 0, 255)); // 파란색 팬 생성
	CPen* pOldPen = dc.SelectObject(&pen);

	CBrush brush(RGB(255, 255, 0)); // 노란색 브러시 생성
	CBrush* pOldBrush = dc.SelectObject(&brush);

	// 랜덤한 위치를 구한 x, y 좌표에 지름 길이 만큼의 width 와 height를 할당한 rect 영역을 지정.
	// 원 내부를 노란색으로 색칠하기
	// 파란색으로 원 가장자리 테두리 그리기

	dc.Ellipse(rectClient);

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);


	CPen pen_center(PS_SOLID, 2, RGB(255, 0, 0)); // 빨간색 펜 생성

	pOldPen = dc.SelectObject(&pen_center);

	// 수직 선 그리기
	dc.MoveTo(m_dCenterX, m_dCenterY);
	// 중심 좌표에서 위로 10 픽셀 만큼 선을 그림
	dc.LineTo(m_dCenterX, m_dCenterY - 10);
	// 중심 좌표에서 아래로 10 픽셀 만큼 선을 그림
	dc.LineTo(m_dCenterX, m_dCenterY + 10);

	// 수평 선 그리기
	dc.MoveTo(m_dCenterX, m_dCenterY);
	// 중심 좌표에서 좌로 10 픽셀 만큼 선을 그림
	dc.LineTo(m_dCenterX - 10, m_dCenterY);
	// 중심 좌표에서 우로 10 픽셀 만큼 선을 그림
	dc.LineTo(m_dCenterX + 10, m_dCenterY);

	dc.SelectObject(pOldPen);
	cout << "원의 무게중심 좌표 : X = " << m_dCenterX << " Y = " << m_dCenterY << endl;

	displayText(&dc, _T("원의 무게중심 좌표"));
}

bool CglimprojDlg::isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
	bool bRet = false;

	double dX = i - nCenterX;
	double dY = j - nCenterY;
	double dDist = dX * dX + dY * dY;

	if (dDist < nRadius * nRadius) {
		bRet = true;
	}

	return bRet;
}

// 실시간으로 현재 좌표에 대한 정보를 화면상단에 표출
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
	// rectClient 영역은 부모 다이얼로그 창을 내부에 위치하므로 부모 다이얼로그 기준으로 rectClient가 위치한 x, y 좌표 값이 rectClient.left, rectClient.top 이다
	// 마우스 클릭 좌표는 부모 다이얼로그 기준으로 산출 되는 x, y 좌표 이므로 rectClient 내부에 맞는 x, y 위치값을 적용하려면 마우스 클릭 좌표 - rectClient가 위치한 x, y 좌표 값을 빼면 내부 상대 좌표값을 구할 수 있음
	strMessage.Format(L"%s (x=%.2f, y=%.2f)", strMsg, m_dCenterX, m_dCenterY);
	pDC->TextOut(rectClient.left + 10, rectClient.top + 10, strMessage);
}


void CglimprojDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1) {
		KillTimer(nIDEvent);

		CRect rectClient;
		// 전체 윈도우 기준으로 m_GroundStatic의 실제 영역 좌표
		m_GroundStatic.GetWindowRect(rectClient);
		// 현재 다이얼로그 기준으로 m_GroundStatic가 위치한 상대적인 영역 좌표
		ScreenToClient(rectClient);

		int nWidth = rectClient.Width();
		int nHeight = rectClient.Height();
		int nBpp = 8;

		// 원이 그려질 Client 영역을 흰색으로 Setting
		m_image.Create(nWidth, -nHeight, nBpp);
		if (nBpp == 8) {
			static RGBQUAD rgb[256];
			for (int i = 0; i < 256; i++)
				rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
			m_image.SetColorTable(0, 256, rgb);
		}

		int nPitch = m_image.GetPitch();
		unsigned char* fm = (unsigned char*)m_image.GetBits();

		memset(fm, 0xff, nWidth * nHeight);

		CClientDC dc(this);

		// m_image의 클라이언트 영역을 그려줌
		m_image.Draw(dc, rectClient.left, rectClient.top);
	}
	CDialogEx::OnTimer(nIDEvent);
}
