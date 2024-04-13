RootWindowHostWin::RootWindowHostWin(const gfx::Rect& bounds)
    : root_window_(NULL),
      fullscreen_(false),
      has_capture_(false),
      saved_window_style_(0),
      saved_window_ex_style_(0) {
  Init(NULL, bounds);
  SetWindowText(hwnd(), L"aura::RootWindow!");
  prop_.reset(new ui::ViewProp(hwnd(), kRootWindowHostWinKey, this));
}
