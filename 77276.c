string16 OmniboxViewWin::GetText() const {
  const int len = GetTextLength() + 1;
  string16 str;
  if (len > 1)
    GetWindowText(WriteInto(&str, len), len);
  return str;
}
