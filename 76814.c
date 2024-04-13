RootWindowHostLinux::~RootWindowHostLinux() {
  static_cast<DispatcherLinux*>(Env::GetInstance()->GetDispatcher())->
      WindowDispatcherDestroying(xwindow_);

  UnConfineCursor();

  XDestroyWindow(xdisplay_, xwindow_);

  ui::GetXCursor(ui::kCursorClearXCursorCache);

  XFreeCursor(xdisplay_, invisible_cursor_);
}
