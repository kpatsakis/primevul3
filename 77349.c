void OmniboxViewWin::TrackMousePosition(MouseButton button,
                                        const CPoint& point) {
  if (gaining_focus_.get()) {
    tracking_click_[button] = true;
    click_point_[button] = point;
  }
}
