void DevToolsWindow::MoveWindow(int x, int y) {
  if (!IsDocked()) {
    gfx::Rect bounds = browser_->window()->GetBounds();
    bounds.Offset(x, y);
    browser_->window()->SetBounds(bounds);
  }
}
