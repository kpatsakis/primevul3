bool OmniboxEditModel::IsPasteAndSearch(const base::string16& text) const {
  AutocompleteMatch match;
  ClassifyStringForPasteAndGo(text, &match, NULL);
  return AutocompleteMatch::IsSearchType(match.type);
}
