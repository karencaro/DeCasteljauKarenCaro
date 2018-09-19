// ChildView.h : interface of the CChildView class
//


#pragma once


// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
   CPoint pt_control[4]; //para almacenar los puntos de control
   int capturado; //para controlar que punto de control se esta capturando
                //si capturado esta entre 0 y 3, aun se esta capturando
               //la curva, si es mayor que 3, ya se termino de definir
               //los 4 puntos de control
   int modificando; //punto de control a modificar
// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// Dibuja un marcador en la posicion pt. Servira para dibujar los puntos de control
	void DibujarMarcador(CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// //dibuja la curva de bezier basado en la ecuacion cubica
	void Bezier(CPoint  pc[4], CDC * pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void DrawDeCasteljau(CPoint p[4], COLORREF color, CDC * pDC);
};

