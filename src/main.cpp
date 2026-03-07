#ifndef UNICODE
#define UNICODE
#endif

#ifndef ID_EDITCHILD
#define ID_EDITCHILD 100
#endif 

#define PRABH_MENU_OPEN_ID 1000
#define PRABH_MENU_SAVE_ID 1001

#include <windows.h>
#include <iostream>

HWND PrabhTextEditor_CreateMultiLineEditControl(HWND parent);
HMENU PrabhTextEditor_GetMenu();

static HWND hwndEditMultiLine;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_CREATE: {
      hwndEditMultiLine = PrabhTextEditor_CreateMultiLineEditControl(hwnd);
      HMENU menu = PrabhTextEditor_GetMenu();
      SetMenu(hwnd, menu);
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

HMENU PrabhTextEditor_GetMenu() {
  HMENU menu = CreateMenu();
  AppendMenu(menu, MF_STRING, PRABH_MENU_OPEN_ID, L"Open");
  AppendMenu(menu, MF_STRING, PRABH_MENU_SAVE_ID, L"Save");
  return menu;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
  const wchar_t CLASS_NAME[] = TEXT("Prabhjot Text Editor");

  WNDCLASS wc = {};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  RegisterClass(&wc);

  // the XOR is to prevent resizing
  // https://stackoverflow.com/questions/3275989/disable-window-resizing-win32
  HWND hwnd = CreateWindowEx(0, CLASS_NAME, CLASS_NAME, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  ShowWindow(hwnd, nCmdShow);

  MSG msg = { };
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

