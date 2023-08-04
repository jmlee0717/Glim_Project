
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
	void drawData(CDC* pDC);
	void drawCircle(CDC* pDC, CRect rect);
	void drawCenter(CDC* pDC);
	void displayText(CDC* pDC, CString strMsg);

	CImage		m_image;
	int			m_nRadius;
	CPoint		m_CenterPt;
	int			m_nPointx, m_nPointy;
	CStatic		m_GroundStatic;
	BOOL		m_bDrawFlag = FALSE;
	CRect		m_rectClient;

};
