bool RootWindowHostWin::ConfineCursorToRootWindow() {
  RECT window_rect;
  GetWindowRect(hwnd(), &window_rect);
   return ClipCursor(&window_rect) != 0;
 }
