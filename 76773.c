void RootWindow::SetCursor(gfx::NativeCursor cursor) {
  last_cursor_ = cursor;
  host_->SetCursor(cursor);
}
