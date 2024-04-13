void RootWindow::SetHostBounds(const gfx::Rect& bounds_in_pixel) {
  DispatchHeldMouseMove();
  host_->SetBounds(bounds_in_pixel);
  last_mouse_location_ =
      ui::ConvertPointToDIP(layer(), host_->QueryMouseLocation());
  synthesize_mouse_move_ = false;
}
