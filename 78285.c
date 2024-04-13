AutocompleteInput::PageClassification OmniboxEditModel::ClassifyPage() const {
  if (!delegate_->CurrentPageExists())
    return AutocompleteInput::OTHER;
  if (delegate_->IsInstantNTP()) {
    return (focus_source_ == FAKEBOX) ?
        AutocompleteInput::INSTANT_NTP_WITH_FAKEBOX_AS_STARTING_FOCUS :
        AutocompleteInput::INSTANT_NTP_WITH_OMNIBOX_AS_STARTING_FOCUS;
  }
  const GURL& gurl = delegate_->GetURL();
  if (!gurl.is_valid())
    return AutocompleteInput::INVALID_SPEC;
  const std::string& url = gurl.spec();
  if (url == chrome::kChromeUINewTabURL)
    return AutocompleteInput::NTP;
  if (url == content::kAboutBlankURL)
    return AutocompleteInput::BLANK;
  if (url == profile()->GetPrefs()->GetString(prefs::kHomePage))
    return AutocompleteInput::HOME_PAGE;
  if (controller_->GetToolbarModel()->WouldPerformSearchTermReplacement(true))
    return AutocompleteInput::SEARCH_RESULT_PAGE_DOING_SEARCH_TERM_REPLACEMENT;
  if (delegate_->IsSearchResultsPage())
    return AutocompleteInput::SEARCH_RESULT_PAGE_NO_SEARCH_TERM_REPLACEMENT;
  return AutocompleteInput::OTHER;
}
