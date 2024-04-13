void OmniboxViewWin::SetWindowTextAndCaretPos(const string16& text,
                                              size_t caret_pos) {
  SetWindowText(text.c_str());
  PlaceCaretAt(caret_pos);
}
