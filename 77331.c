void OmniboxViewWin::PlaceCaretAt(string16::size_type pos) {
  SetSelection(static_cast<LONG>(pos), static_cast<LONG>(pos));
}
