int OmniboxViewWin::WidthOfTextAfterCursor() {
  CHARRANGE selection;
  GetSelection(selection);
  const int start = std::max(0, static_cast<int>(selection.cpMax - 1));
  return WidthNeededToDisplay(GetText().substr(start));
}
