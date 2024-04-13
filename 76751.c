void RootWindow::MoveCursorTo(const gfx::Point& location_in_dip) {
  host_->MoveCursorTo(ui::ConvertPointToPixel(layer(), location_in_dip));
}
