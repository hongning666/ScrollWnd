#include<windows.h>

TCHAR* szClassName = L"ScrollWndDemo";


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



int WinMain(HINSTANCE hInstance,
	HINSTANCE phInstance,
	PSTR ptr,
	int show)
{
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = szClassName;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, L"×¢²á´°¿ÚÊ§°Ü£¡", L"´íÎó£¡", MB_ICONERROR | MB_OK);
	}
	int xClient = GetSystemMetrics(SM_CXSCREEN);
	int yClient = GetSystemMetrics(SM_CYSCREEN);
	HWND hWnd = CreateWindow(szClassName, L"¹ö¶¯Ìõ´°¿Ú", WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, xClient / 4, yClient / 3, 600, 400, NULL, NULL, hInstance, 0);

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hHBitmap;
	static BITMAP  BitInfo;
	static int iHScrollPos, iVScrollPos;
	static int iXClient, iYClient;
	static int iBitmapWidth, iBitmapHeight;
	switch (message)
	{
	case WM_CREATE:
		hHBitmap = (HBITMAP)LoadImage(NULL, L"Bitmap.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (!hHBitmap)
			MessageBox(hWnd, L"¼ÓÔØÎ»Í¼Ê§°Ü£¡", L"´íÎó£¡", MB_ICONERROR);
		GetObject(hHBitmap, sizeof(BITMAP), &BitInfo);
		iBitmapWidth = BitInfo.bmWidth;
		iBitmapHeight = BitInfo.bmHeight;
		break;
	case WM_PAINT:
		HDC hdc, memdc;
		PAINTSTRUCT ps;
		hdc=BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hHBitmap);
		BitBlt(hdc, -iHScrollPos, -iVScrollPos, iBitmapWidth, iBitmapHeight, memdc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		DeleteDC(memdc);
		break;
	case WM_SIZE:
		iXClient = LOWORD(lParam);
		iYClient = HIWORD(lParam);
		//ÉèÖÃ¹ö¶¯Ìõ·¶Î§
		SetScrollRange(hWnd, SB_HORZ, 0, iBitmapWidth - iXClient, FALSE);
		SetScrollRange(hWnd, SB_VERT, 0, iBitmapHeight - iYClient, FALSE);
		break;
	case WM_HSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			iHScrollPos -= 5;
			break;
		case SB_LINEDOWN:
			iHScrollPos += 5;
			break;
		case SB_PAGEUP:
			iHScrollPos -= iXClient;
			break;
		case SB_PAGEDOWN:
			iHScrollPos += iXClient;
			break;
		case SB_THUMBTRACK:
			iHScrollPos = HIWORD(wParam);
			break;
		}
		iHScrollPos = min(iBitmapWidth - iXClient, max(0,iHScrollPos));
		SetScrollPos(hWnd, SB_HORZ, iHScrollPos, TRUE);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			iVScrollPos -= 5;
			break;
		case SB_LINEDOWN:
			iVScrollPos += 5;
			break;
		case SB_PAGEUP:
			iVScrollPos -= iYClient;
			break;
		case SB_PAGEDOWN:
			iVScrollPos += iYClient;
			break;
		case SB_THUMBTRACK:
			iVScrollPos = HIWORD(wParam);
			break;
		}
		iVScrollPos = min(iBitmapHeight - iYClient, max(0,iVScrollPos));
		SetScrollPos(hWnd, SB_VERT, iVScrollPos, TRUE);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}