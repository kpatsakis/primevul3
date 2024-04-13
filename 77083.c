gfx::Rect DesktopNativeWidgetHelperAura::ModifyAndSetBounds(
    const gfx::Rect& bounds) {
  gfx::Rect out_bounds = bounds;
  if (root_window_.get() && !out_bounds.IsEmpty()) {
    root_window_->SetHostBounds(out_bounds);
    out_bounds.set_x(0);
    out_bounds.set_y(0);
  } else if (is_embedded_window_) {
    aura::RootWindow* root =
        widget_->GetNativeWindow()->GetRootWindow()->GetRootWindow();
    gfx::Point point = root->GetHostOrigin();
    out_bounds.set_x(out_bounds.x() - point.x());
    out_bounds.set_y(out_bounds.y() - point.y());
  }

  return out_bounds;
}
