void OmniboxViewWin::InsertText(int position, const string16& text) {
  DCHECK((position >= 0) && (position <= GetTextLength()));
  ScopedFreeze freeze(this, GetTextObjectModel());
  OnBeforePossibleChange();
  SetSelection(position, position);
  ReplaceSel(text.c_str());
  OnAfterPossibleChange();
}
