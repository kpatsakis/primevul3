void RootWindowHostWin::MoveCursorTo(const gfx::Point& location) {
  POINT pt;
  ClientToScreen(hwnd(), &pt);
  SetCursorPos(pt.x, pt.y);
}
