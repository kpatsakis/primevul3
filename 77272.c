string16 OmniboxViewWin::GetSelectedText() const {
  CHARRANGE sel;
  GetSel(sel);
  string16 str;
  if (sel.cpMin != sel.cpMax)
    GetSelText(WriteInto(&str, sel.cpMax - sel.cpMin + 1));
  return str;
}
