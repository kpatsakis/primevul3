void OmniboxViewWin::SelectAllIfNecessary(MouseButton button,
                                          const CPoint& point) {
  if (tracking_click_[button] &&
      !IsDrag(click_point_[button], point)) {
    SelectAll(true);
    possible_drag_ = false;
  }
}
