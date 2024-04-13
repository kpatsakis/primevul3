void RootWindow::Init() {
  compositor()->SetScaleAndSize(GetDeviceScaleFactorFromMonitor(this),
                                host_->GetBounds().size());
  Window::Init(ui::LAYER_NOT_DRAWN);
  last_mouse_location_ =
      ui::ConvertPointToDIP(layer(), host_->QueryMouseLocation());
  compositor()->SetRootLayer(layer());
  SetBounds(
      ui::ConvertRectToDIP(layer(), gfx::Rect(host_->GetBounds().size())));
  Show();
  host_->SetRootWindow(this);
}
