int OmniboxViewWin::WidthNeededToDisplay(const string16& text) const {
  return font_.GetStringWidth(text) + GetHorizontalMargin();
}
