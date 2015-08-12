
#include <Windows.h>
#include "resource.h"
#include "imageproc.h"

HBITMAP hBitmapOriginal;
HBITMAP hBitmapEdited;

BOOL CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE, PSTR, int)
{
	hBitmapOriginal = (HBITMAP)LoadImage(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	BITMAP header;
	GetObject(hBitmapOriginal, sizeof(BITMAP), &header);

	hBitmapEdited = CreateBitmap(header.bmWidth, header.bmHeight, header.bmPlanes, header.bmBitsPixel, NULL);
	
	//test
	

	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, WndProc);
	DeleteObject(hBitmapOriginal);
	DeleteObject(hBitmapEdited);
	return 0;
}

BOOL CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hWnd, IDC_IMAGE), STM_SETIMAGE, IMAGE_BITMAP, (LRESULT)hBitmapOriginal);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK)
		{
			HDC hDc = GetDC(hWnd);
			BITMAPINFO info = { 0 };
			info.bmiHeader.biSize = sizeof(info.bmiHeader);

			GetDIBits(hDc, hBitmapOriginal, 0, 0, NULL, &info, DIB_RGB_COLORS);
			int bufSize = info.bmiHeader.biHeight * info.bmiHeader.biWidth * (info.bmiHeader.biBitCount / 8);
			unsigned char *buf = new unsigned char[bufSize];
			
			
			
			
			info.bmiHeader.biCompression = 0;
			GetDIBits(hDc, hBitmapOriginal, 0, info.bmiHeader.biHeight, buf, &info, DIB_RGB_COLORS);
			Texture t = CreateTextureFromRawData(buf, info.bmiHeader.biHeight, info.bmiHeader.biWidth, info.bmiHeader.biBitCount / 8, 0, 1, 2);
			memset(buf, 0, bufSize);
			float blurKernel[] =
			{
				0.11111f, 0.11111f, 0.11111f,
				0.11111f, 0.11111f, 0.11111f,
				0.11111f, 0.11111f, 0.11111f
			};
			Texture t1;
			PixelShader(t, &t1, KernelShader, blurKernel, 3);
			TextureToRawData(t1, buf, info.bmiHeader.biHeight, info.bmiHeader.biWidth, info.bmiHeader.biBitCount / 8, 0, 1, 2);
			DeleteTexture(t);
			DeleteTexture(t1);
			SetDIBits(hDc, hBitmapEdited, 0, info.bmiHeader.biHeight, buf, &info, DIB_RGB_COLORS);
			ReleaseDC(hWnd, hDc);
			delete[] buf;
			SendMessage(GetDlgItem(hWnd, IDC_IMAGE), STM_SETIMAGE, IMAGE_BITMAP, (LRESULT)hBitmapEdited);
		}
	}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}