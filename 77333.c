void OmniboxViewWin::RepaintDropHighlight(int position) {
  if ((position != -1) && (position <= GetTextLength())) {
    const POINT min_loc(PosFromChar(position));
    const RECT highlight_bounds = {min_loc.x - 1, font_y_adjustment_,
        min_loc.x + 2, font_.GetHeight() + font_y_adjustment_};
    InvalidateRect(&highlight_bounds, false);
  }
}
