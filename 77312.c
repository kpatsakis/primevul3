void OmniboxViewWin::OnLButtonUp(UINT keys, const CPoint& point) {
  ScopedFreeze freeze(this, GetTextObjectModel());
  DefWindowProc(WM_LBUTTONUP, keys,
                MAKELPARAM(ClipXCoordToVisibleText(point.x, false), point.y));

  SelectAllIfNecessary(kLeft, point);

  tracking_click_[kLeft] = false;

  possible_drag_ = false;
}
