bool OmniboxEditModel::OnEscapeKeyPressed() {
  const AutocompleteMatch& match = CurrentMatch(NULL);
  if (has_temporary_text_) {
    if (match.destination_url != original_url_) {
      RevertTemporaryText(true);
      return true;
    }
  }

  if (delegate_->CurrentPageExists() && !delegate_->IsLoading()) {
    delegate_->GetNavigationController().DiscardNonCommittedEntries();
    view_->Update();
  }

  bool should_disable_url_replacement =
      controller_->GetToolbarModel()->url_replacement_enabled() &&
      !controller_->GetToolbarModel()->WouldPerformSearchTermReplacement(true);

  const bool has_zero_suggest_match = match.provider &&
      (match.provider->type() == AutocompleteProvider::TYPE_ZERO_SUGGEST);
  if (!has_zero_suggest_match && !should_disable_url_replacement &&
      !user_input_in_progress_ && view_->IsSelectAll())
    return false;

  if (!user_text_.empty()) {
    UMA_HISTOGRAM_ENUMERATION(kOmniboxUserTextClearedHistogram,
                              OMNIBOX_USER_TEXT_CLEARED_WITH_ESCAPE,
                              OMNIBOX_USER_TEXT_CLEARED_NUM_OF_ITEMS);
  }

  if (should_disable_url_replacement) {
    controller_->GetToolbarModel()->set_url_replacement_enabled(false);
    UpdatePermanentText();
  }
  view_->RevertWithoutResettingSearchTermReplacement();
  view_->SelectAll(true);
  return true;
}
