void OmniboxViewWin::SetForcedQuery() {
  const string16 current_text(GetText());
  const size_t start = current_text.find_first_not_of(kWhitespaceWide);
  if (start == string16::npos || (current_text[start] != '?'))
    SetUserText(L"?");
  else
    SetSelection(current_text.length(), start + 1);
}
