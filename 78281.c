void OmniboxEditModel::AcceptInput(WindowOpenDisposition disposition,
                                   bool for_drop) {
  GURL alternate_nav_url;
  AutocompleteMatch match = CurrentMatch(&alternate_nav_url);

  if (control_key_state_ == DOWN_WITHOUT_CHANGE && !KeywordIsSelected() &&
      autocomplete_controller()->history_url_provider()) {
    input_ = AutocompleteInput(
      has_temporary_text_ ?
          UserTextFromDisplayText(view_->GetText())  : input_.text(),
      input_.cursor_position(), base::ASCIIToUTF16("com"),
      GURL(), input_.current_page_classification(),
      input_.prevent_inline_autocomplete(), input_.prefer_keyword(),
      input_.allow_exact_keyword_match(),
      input_.want_asynchronous_matches());
    AutocompleteMatch url_match(
        autocomplete_controller()->history_url_provider()->SuggestExactInput(
            input_.text(), input_.canonicalized_url(), false));


    if (url_match.destination_url.is_valid()) {
      match = url_match;
      alternate_nav_url = GURL();
    }
  }

  if (!match.destination_url.is_valid())
    return;

  if ((match.transition == content::PAGE_TRANSITION_TYPED) &&
      (match.destination_url == PermanentURL())) {
    match.transition = content::PAGE_TRANSITION_RELOAD;
  } else if (for_drop || ((paste_state_ != NONE) &&
                          match.is_history_what_you_typed_match)) {
    match.transition = content::PAGE_TRANSITION_LINK;
  }

  const TemplateURL* template_url = match.GetTemplateURL(profile_, false);
  if (template_url && template_url->url_ref().HasGoogleBaseURLs())
    GoogleURLTracker::GoogleURLSearchCommitted(profile_);

  DCHECK(popup_model());
  view_->OpenMatch(match, disposition, alternate_nav_url, base::string16(),
                   popup_model()->selected_line());
}
