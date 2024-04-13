base::string16 OmniboxEditModel::DisplayTextFromUserText(
    const base::string16& text) const {
  return KeywordIsSelected() ?
      KeywordProvider::SplitReplacementStringFromInput(text, false) : text;
}
