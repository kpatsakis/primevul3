base::string16 OmniboxEditModel::UserTextFromDisplayText(
    const base::string16& text) const {
  return KeywordIsSelected() ? (keyword_ + base::char16(' ') + text) : text;
}
