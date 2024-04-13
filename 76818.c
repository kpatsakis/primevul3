gfx::Rect RootWindowHostWin::GetBounds() const {
  RECT r;
  GetClientRect(hwnd(), &r);
  return gfx::Rect(r);
}
