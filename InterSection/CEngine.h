#pragma once
#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>

#include "CMatrix.h"


#define ID_BUTTON_A 22
#define TIMER1 1001

class Engine
{
public:
	Engine(HINSTANCE);
	~Engine();
	
	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize();

	// Process and dispatch messages
	void RunMessageLoop();

	// The windows procedure.
	static LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	LRESULT Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	struct rvector {
		float x, y;
	};
	struct element {
		rvector pos;
		rvector V;
		float mass;
	};
private:
	HINSTANCE hInst;
	HWND hWnd;

	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1SolidColorBrush* pBrush;
	ID2D1StrokeStyle* pStroke;
	ID2D1GeometrySink* pSink;
	ID2D1PathGeometry* m_pPathGeometry;

	
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateTarget();
	HRESULT Render();
	void DiscardDeviceResources();
	void ResizeTarget();

	void RenderParticle(element *);
	void RenderVector_offset(rvector*, rvector*);

	struct WndButton
	{
		int l;
		int t;
		int h;
		int w;
		int id;
		bool pushed;
	};
	WndButton btnA;

	element Aparticle,Bparticle;
};
