bool OmniboxViewWin::IsSelectAll() {
  CHARRANGE selection;
  GetSel(selection);
  return IsSelectAllForRange(selection);
}
