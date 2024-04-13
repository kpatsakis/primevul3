gfx::Point RootWindowHostWin::QueryMouseLocation() {
  POINT pt;
  GetCursorPos(&pt);
  ScreenToClient(hwnd(), &pt);
  const gfx::Size size = GetBounds().size();
  return gfx::Point(max(0, min(size.width(), static_cast<int>(pt.x))),
                    max(0, min(size.height(), static_cast<int>(pt.y))));
}
