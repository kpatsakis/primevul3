void RootWindowHostWin::ToggleFullScreen() {
  gfx::Rect target_rect;
  if (!fullscreen_) {
    fullscreen_ = true;
    saved_window_style_ = GetWindowLong(hwnd(), GWL_STYLE);
    saved_window_ex_style_ = GetWindowLong(hwnd(), GWL_EXSTYLE);
    GetWindowRect(hwnd(), &saved_window_rect_);
    SetWindowLong(hwnd(), GWL_STYLE,
                  saved_window_style_ & ~(WS_CAPTION | WS_THICKFRAME));
    SetWindowLong(hwnd(), GWL_EXSTYLE,
                  saved_window_ex_style_ & ~(WS_EX_DLGMODALFRAME |
                      WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(MonitorFromWindow(hwnd(), MONITOR_DEFAULTTONEAREST), &mi);
    target_rect = mi.rcMonitor;
  } else {
    fullscreen_ = false;
    SetWindowLong(hwnd(), GWL_STYLE, saved_window_style_);
    SetWindowLong(hwnd(), GWL_EXSTYLE, saved_window_ex_style_);
    target_rect = saved_window_rect_;
  }
  SetWindowPos(hwnd(),
               NULL,
               target_rect.x(),
               target_rect.y(),
               target_rect.width(),
               target_rect.height(),
               SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
}
