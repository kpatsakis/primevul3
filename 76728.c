void RootWindow::ConvertPointToNativeScreen(gfx::Point* point) const {
  gfx::Point location = host_->GetLocationOnNativeScreen();
  point->Offset(location.x(), location.y());
}
