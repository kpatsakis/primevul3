bool OmniboxViewWin::IsCaretAtEnd() const {
  long length = GetTextLength();
  CHARRANGE sel;
  GetSelection(sel);
  return sel.cpMin == sel.cpMax && sel.cpMin == length;
}
