void OmniboxViewWin::GetSelectionBounds(string16::size_type* start,
                                        string16::size_type* end) const {
  CHARRANGE selection;
  GetSel(selection);
  *start = static_cast<size_t>(selection.cpMin);
  *end = static_cast<size_t>(selection.cpMax);
}
