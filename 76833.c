void RootWindowHostWin::SetBounds(const gfx::Rect& bounds) {
  if (fullscreen_) {
    saved_window_rect_.right = saved_window_rect_.left + bounds.width();
    saved_window_rect_.bottom = saved_window_rect_.top + bounds.height();
    return;
  }
  RECT window_rect;
  window_rect.left = bounds.x();
  window_rect.top = bounds.y();
  window_rect.right = bounds.right() ;
  window_rect.bottom = bounds.bottom();
  AdjustWindowRectEx(&window_rect,
                     GetWindowLong(hwnd(), GWL_STYLE),
                     FALSE,
                     GetWindowLong(hwnd(), GWL_EXSTYLE));
  SetWindowPos(
      hwnd(),
      NULL,
      0,
      0,
      window_rect.right - window_rect.left,
      window_rect.bottom - window_rect.top,
      SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOREPOSITION);
}
