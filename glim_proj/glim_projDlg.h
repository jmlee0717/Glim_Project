
// glim_projDlg.h: 헤더 파일
//

#pragma once


// CglimprojDlg 대화 상자
class CglimprojDlg : public CDialogEx
{
// 생성입니다.
public:
	CglimprojDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIM_PROJ_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDrawBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	// 원을 그리기 위한 rect의 Start 좌표를 랜덤으로 구하고 클라이언트 영역을 벗어나면 예외처리하는 함수
	void drawData(CDC* pDC);
	// 원의 무게 중심을 표출 하는 함수
	void displayText(CDC* pDC, CString strMsg);
	// 원을 그리고 무게중심좌표를 그리는 함수
	void drawCircle(unsigned char* fm, int i, int j, int nRadius, int nGray);
	// 원의 영역안에 좌표를 체크
	bool isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);

	CImage		m_image;
	int			m_nSize;				// 원의 크기(지름) 
	double		m_dCenterX;				// 무게 중심 X 좌표	
	double		m_dCenterY;				// 무게 중심 Y 좌표
	int			m_nPointx, m_nPointy;	// 원을 그리기 위한 사각형의 시작 x, y 좌표
	CStatic		m_GroundStatic;			// 다이얼로그 안에 원을 그리기 위한 영역		
	BOOL		m_bDrawFlag = FALSE;	// OnPaint에서 원을 그릴지 판단
};
