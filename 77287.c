bool OmniboxViewWin::IsSelectAllForRange(const CHARRANGE& sel) const {
  const int text_length = GetTextLength();
  return ((sel.cpMin == 0) && (sel.cpMax >= text_length)) ||
      ((sel.cpMax == 0) && (sel.cpMin >= text_length));
}
