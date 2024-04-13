void RootWindowHostWin::SetCursor(gfx::NativeCursor native_cursor) {
  if (native_cursor == ui::kCursorCustom)
    return;
  const wchar_t* cursor_id = GetCursorId(native_cursor);
  ::SetCursor(LoadCursor(NULL, cursor_id));
}
