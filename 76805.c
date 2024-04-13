void RootWindowHostLinux::SetBounds(const gfx::Rect& bounds) {
  bool size_changed = bounds_.size() != bounds.size();
  if (bounds == bounds_) {
    root_window_->SchedulePaintInRect(root_window_->bounds());
    return;
  }

  if (bounds.size() != bounds_.size())
    XResizeWindow(xdisplay_, xwindow_, bounds.width(), bounds.height());

  if (bounds.origin() != bounds_.origin())
    XMoveWindow(xdisplay_, xwindow_, bounds.x(), bounds.y());

  bounds_ = bounds;
  if (size_changed)
    root_window_->OnHostResized(bounds.size());
}
