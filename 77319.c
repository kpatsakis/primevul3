void OmniboxViewWin::OnPossibleDrag(const CPoint& point) {
  if (possible_drag_)
    return;

  click_point_[kLeft] = point;
  initiated_drag_ = false;

  CHARRANGE selection;
  GetSel(selection);
  if (selection.cpMin != selection.cpMax) {
    const POINT min_sel_location(PosFromChar(selection.cpMin));
    const POINT max_sel_location(PosFromChar(selection.cpMax));
    possible_drag_ = (point.x >= min_sel_location.x) &&
                     (point.x < max_sel_location.x);
  }
}
