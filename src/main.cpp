#ifndef UNICODE
#define UNICODE
#endif

#ifndef ID_EDITCHILD
#define ID_EDITCHILD 100
#endif 

#include <windows.h>
#include <iostream>

HWND PrabhTextEditor_CreateMultiLineEditControl(HWND parent);

static HWND hwndEditMultiLine;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_CREATE: {
      hwndEditMultiLine = PrabhTextEditor_CreateMultiLineEditControl(hwnd);
      return 0;
    }
    case WM_SIZE: {
      MoveWindow(hwndEditMultiLine, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
      return 0;
    }
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
      EndPaint(hwnd, &ps);
      return 0;
    }
    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }
    default:
      return DefWindowProc(hwnd, uMsg, wParam, lParam);  
  }
  return 0; 
}

// https://learn.microsoft.com/en-us/windows/win32/controls/use-a-multiline-edit-control
HWND PrabhTextEditor_CreateMultiLineEditControl(HWND parent) {
  HWND hwndEdit = CreateWindowEx(
    0, 
    L"EDIT", 
    NULL, 
    WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
    0, 0, 0, 0, 
    parent, 
    (HMENU) ID_EDITCHILD, 
    (HINSTANCE) GetWindowLongPtr(parent, GWLP_HINSTANCE), 
    NULL
  );
  return hwndEdit;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
  const wchar_t CLASS_NAME[] = TEXT("Prabhjot Text Editor");

  WNDCLASS wc = {};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  RegisterClass(&wc);

  HWND hwnd = CreateWindowEx(0, CLASS_NAME, CLASS_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  ShowWindow(hwnd, nCmdShow);

  MSG msg = { };
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

