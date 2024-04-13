void OmniboxViewWin::OnLButtonDown(UINT keys, const CPoint& point) {
  TrackMousePosition(kLeft, point);
  if (gaining_focus_.get()) {
    saved_selection_for_focus_change_.cpMin = -1;

    DefWindowProc(WM_LBUTTONDOWN, keys, MAKELPARAM(point.x, point.y));
    DefWindowProc(WM_LBUTTONUP, keys, MAKELPARAM(point.x, point.y));
  }

  const bool is_triple_click = tracking_double_click_ &&
      views::NativeTextfieldWin::IsDoubleClick(double_click_point_, point,
          GetCurrentMessage()->time - double_click_time_);
  tracking_double_click_ = false;

  if (!gaining_focus_.get() && !is_triple_click)
    OnPossibleDrag(point);


  ScopedFreeze freeze(this, GetTextObjectModel());
  OnBeforePossibleChange();
  DefWindowProc(WM_LBUTTONDOWN, keys,
                MAKELPARAM(ClipXCoordToVisibleText(point.x, is_triple_click),
                           point.y));
  OnAfterPossibleChange();

  gaining_focus_.reset();
}
