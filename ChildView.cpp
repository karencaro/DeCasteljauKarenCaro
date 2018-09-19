// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "DeCasteljau.h"
#include "ChildView.h"
#include<math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	capturado=-1;//para indicar que no se ha
	             //capturado nada
	modificando=-1;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
	//se dibujan los puntos de control
	for(int i=0;i<capturado;i++)
		DibujarMarcador(pt_control[i]);
	
	if(capturado<4) return;
    //Dibujar la curva de bezier
	DrawDeCasteljau(pt_control,RGB(0,0,0),&dc);
}


void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CWnd::OnLButtonDblClk(nFlags, point);
	//se comenzara a capturar el poligono
	if(capturado<0||capturado>3){
		//se define el primer punto de control
      pt_control[0]=point;
	  capturado=1; // se ha capturado 1 punto
	  //dibuja un marcador en el primer punto de control
	  //DibujarMarcador(pt_control[0]);
	  RedrawWindow();
	}

}

// Dibuja un marcador en la posicion pt. Servira para dibujar los puntos de control
void CChildView::DibujarMarcador(CPoint pt)
{
	CDC *pDC=GetDC(); //obtiene el contexto de la ventana para dibujar
    //selecciona la pluma estandar para dibujar contornos
	pDC->SelectStockObject(DC_PEN);
	//selecciona la brocha estandar para rellenar figuras
	pDC->SelectStockObject(DC_BRUSH);
	pDC->SetDCPenColor(RGB(255,0,0)); //pone la pluma en rojo
	pDC->SetDCBrushColor(RGB(255,0,0)); //pone la brocha en rojo

	pDC->Ellipse(pt.x-3,pt.y-3,pt.x+3,pt.y+3);
	ReleaseDC(pDC); //libera el contexrto de la ventana
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CWnd::OnLButtonDown(nFlags, point);
	//si ya se comenzo la captura, y aun no se han capturado
	//4 puntos de control
	if(capturado>0&&capturado<4){
		pt_control[capturado]=point;
		DibujarMarcador(pt_control[capturado++]);
		if(capturado==4)
			RedrawWindow();
		return;
	}
	if(capturado<0)return;
	//verifica si se dio click en alguno de los 4 puntos de control
	for(int i=0;i<4;i++){
		//rectangulo que envuelve la elipse del marcador
        CRect mkr(pt_control[i].x-3,pt_control[i].y-3,
			      pt_control[i].x+3,pt_control[i].y+3);
		//si el punto en donde se dio click esta dentro del rectangulo
		//quieren manipular ese punto de control
		if(mkr.PtInRect(point)){ 
			modificando=i;
			return;
		}
	}
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CWnd::OnLButtonUp(nFlags, point);
	modificando=-1;//se deja de modificar marcador
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CWnd::OnMouseMove(nFlags, point);
	//si estan modificando un marcador
	//y esta presionado el boton izquierdo
	if(modificando>-1&&nFlags&MK_LBUTTON){
        pt_control[modificando]=point;
		RedrawWindow();
	}
}
/*
void CChildView::DrawBezie(CPoint p[4], COLORREF color, CDC * pDC)
{
	//Configuramos color
	pDC->SelectStockObject(DC_PEN);
	pDC->SetDCPenColor(color);
	float t=0.0f;// parametro t de la curva de bezier
	float dt=1.0/20; //delta t para una curva de 20 segmentos
	CPoint pt; //punto calculado para t
	pDC->MoveTo(p[0]); //curva comienza en el primer punto de control}
	t=dt;
	while(t<1.0){
	//calculamos pt
		pt.x=(pow(1-t,3)*p[0].x)+(3*pow(1-t,2)*t*p[1].x)+(3*(1-t)*pow(t,2)*p[2].x)+(pow(t,3)*p[3].x);
		pt.y=(pow(1-t,3)*p[0].y)+(3*pow(1-t,2)*t*p[1].y)+(3*(1-t)*pow(t,2)*p[2].y)+(pow(t,3)*p[3].y);
		pDC->LineTo(pt);
		//incrementamos t
		t+=dt;


	}
	pDC->LineTo(p[3]);



}*/


void CChildView::DrawDeCasteljau(CPoint p[4], COLORREF color, CDC * pDC)
{
	//Configuramos color
	pDC->SelectStockObject(DC_PEN);
	pDC->SetDCPenColor(color);
	float t=0.0f;// parametro t de la curva de bezier
	float dt=1.0/20; //delta t para una curva de 20 segmentos
	CPoint Q0; //punto calculado para t
	CPoint Q1;
	CPoint Q2;
	CPoint pt;
	CPoint R0;
	CPoint R1;
	

	pDC->MoveTo(p[0]); //curva comienza en el primer punto de control}
	t=dt;
	while(t<1.0){
	//calculamos pt

		//Q0=P0+((P1-P0)*t)
		//Necesitara calcular Q0,Q1 y Q2 primero, en donde Q0 es la interpolacion entre P0 y P1 dado el parametro t. 
		//Q1 es la interpolacion entre P1 y P2, y Q2 es la interpolacion entre P2 y P3. 
		//La interpolacion en de P0 y P1 esta dada por: Q0=P0+((P1-P0)*t)
		//Q0=P0+((P1-P0)*t)
		//Q1=P1+((P2-P1)*t)
		//Q2=P2+((P3-P2)*t)
		/////////////////////////////////////////////////////////////////////////////////////////////
		Q0.x=p[0].x+((p[1].x-p[0].x)*t);
		Q0.y=p[0].y+((p[1].y-p[0].y)*t);
		Q1.x=p[1].x+((p[2].x-p[1].x)*t);
		Q1.y=p[1].y+((p[2].y-p[1].y)*t);
		Q2.x=p[2].x+((p[3].x-p[2].x)*t);
		Q2.y=p[2].y+((p[3].y-p[2].y)*t);

		R0.x=Q0.x+((Q1.x-Q0.x)*t);
		R0.y=Q0.y+((Q1.y-Q0.y)*t);
		R1.x=Q1.x+((Q2.x-Q1.x)*t);
		R1.y=Q1.y+((Q2.y-Q1.y)*t);

		pt.x=R0.x+((R1.x-R0.x)*t);
		pt.y=R0.y+((R1.y-R0.y)*t);
		/////////////////////////////////////////////////////////////////////////////////////////////
		//pt.x=(pow(1-t,3)*p[0].x)+(3*pow(1-t,2)*t*p[1].x)+(3*(1-t)*pow(t,2)*p[2].x)+(pow(t,3)*p[3].x);
		//pt.y=(pow(1-t,3)*p[0].y)+(3*pow(1-t,2)*t*p[1].y)+(3*(1-t)*pow(t,2)*p[2].y)+(pow(t,3)*p[3].y);
	
		pDC->LineTo(pt);

		//incrementamos t
		t+=dt;

	}
	pDC->LineTo(p[3]);

}
