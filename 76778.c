void RootWindow::SetHostSize(const gfx::Size& size_in_pixel) {
  DispatchHeldMouseMove();
  gfx::Rect bounds = host_->GetBounds();
  bounds.set_size(size_in_pixel);
  host_->SetBounds(bounds);
  last_mouse_location_ =
      ui::ConvertPointToDIP(layer(), host_->QueryMouseLocation());
  synthesize_mouse_move_ = false;
}
