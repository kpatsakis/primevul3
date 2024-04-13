AutocompleteMatch::Type OmniboxEditModel::CurrentTextType() const {
  return CurrentMatch(NULL).type;
}
