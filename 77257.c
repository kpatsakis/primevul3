BOOL WINAPI EndPaintIntercept(HWND hWnd, const PAINTSTRUCT* lpPaint) {
  return (edit_hwnd && (hWnd == edit_hwnd)) || ::EndPaint(hWnd, lpPaint);
}
