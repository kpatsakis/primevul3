void OmniboxViewWin::SelectAll(bool reversed) {
  if (reversed)
    SetSelection(GetTextLength(), 0);
  else
    SetSelection(0, GetTextLength());
}
