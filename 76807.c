void RootWindowHostLinux::SetCursor(gfx::NativeCursor cursor) {
  if (cursor == current_cursor_)
    return;
  current_cursor_ = cursor;

  if (cursor_shown_)
    SetCursorInternal(cursor);
}
