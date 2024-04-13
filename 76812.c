void RootWindowHostLinux::ShowCursor(bool show) {
  if (show == cursor_shown_)
    return;
  cursor_shown_ = show;
  SetCursorInternal(show ? current_cursor_ : ui::kCursorNone);
}
