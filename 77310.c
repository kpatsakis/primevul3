void OmniboxViewWin::OnLButtonDblClk(UINT keys, const CPoint& point) {
  tracking_double_click_ = true;
  double_click_point_ = point;
  double_click_time_ = GetCurrentMessage()->time;
  possible_drag_ = false;

  ScopedFreeze freeze(this, GetTextObjectModel());
  OnBeforePossibleChange();
  DefWindowProc(WM_LBUTTONDBLCLK, keys,
                MAKELPARAM(ClipXCoordToVisibleText(point.x, false), point.y));
  OnAfterPossibleChange();

  gaining_focus_.reset();  // See NOTE in OnMouseActivate().
}
