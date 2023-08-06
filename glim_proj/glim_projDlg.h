
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	// 원을 그리고 중심좌표를 그리는 메인 함수
	void drawData(CDC* pDC);
	// 원을 그리고 색칠하는 메인 함수
	void drawCircle(CDC* pDC, CRect rect);
	// 원의 중심을 표출하는 함수
	void drawCenter(CDC* pDC);
	// 실시간으로 현재 좌표에 대한 정보를 화면 상단에 표출
	void displayText(CDC* pDC, CString strMsg);

	CImage		m_image;
	int			m_nSize;				// 원의 크기(지름) 
	CPoint		m_CenterPt;				// 원의 중심좌표 
	int			m_nPointx, m_nPointy;	// 원을 그리기 위한 사각형의 시작 x, y 좌표
	CStatic		m_GroundStatic;			// 다이얼로그 안에 원을 그리기 위한 영역
	BOOL		m_bDrawFlag = FALSE;	// OnPaint에서 원을 그릴지 판단
};
