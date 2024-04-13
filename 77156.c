bool NetworkActionPredictor::IsPreconnectable(const AutocompleteMatch& match) {
  switch (match.type) {
    case AutocompleteMatch::SEARCH_WHAT_YOU_TYPED:
    case AutocompleteMatch::SEARCH_HISTORY:
    case AutocompleteMatch::SEARCH_SUGGEST:
    case AutocompleteMatch::SEARCH_OTHER_ENGINE:
      return true;

    default:
      return false;
  }
}
