void OmniboxEditModel::PasteAndGo(const base::string16& text) {
  DCHECK(CanPasteAndGo(text));
  UMA_HISTOGRAM_COUNTS("Omnibox.PasteAndGo", 1);

  view_->RevertAll();
  AutocompleteMatch match;
  GURL alternate_nav_url;
  ClassifyStringForPasteAndGo(text, &match, &alternate_nav_url);
  view_->OpenMatch(match, CURRENT_TAB, alternate_nav_url, text,
                   OmniboxPopupModel::kNoMatch);
}
