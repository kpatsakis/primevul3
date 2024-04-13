void OmniboxEditModel::GetInfoForCurrentText(AutocompleteMatch* match,
                                             GURL* alternate_nav_url) const {
  DCHECK(match != NULL);

  if (controller_->GetToolbarModel()->WouldPerformSearchTermReplacement(
      false)) {

    match->type = AutocompleteMatchType::SEARCH_WHAT_YOU_TYPED;
    match->provider = autocomplete_controller()->search_provider();
    match->destination_url =
        delegate_->GetNavigationController().GetVisibleEntry()->GetURL();
    match->transition = content::PAGE_TRANSITION_RELOAD;
  } else if (query_in_progress() ||
             (popup_model() && popup_model()->IsOpen())) {
    if (query_in_progress()) {
      if (result().empty())
        return;
      *match = *result().default_match();
    } else {
      CHECK(!result().empty());
      CHECK(popup_model()->selected_line() < result().size());
      *match = result().match_at(popup_model()->selected_line());
    }
    if (alternate_nav_url &&
        (!popup_model() || popup_model()->manually_selected_match().empty()))
      *alternate_nav_url = result().alternate_nav_url();
  } else {
    AutocompleteClassifierFactory::GetForProfile(profile_)->Classify(
        UserTextFromDisplayText(view_->GetText()), KeywordIsSelected(), true,
        ClassifyPage(), match, alternate_nav_url);
  }
}
