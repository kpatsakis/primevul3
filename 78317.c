void OmniboxEditModel::StartAutocomplete(
    bool has_selected_text,
    bool prevent_inline_autocomplete) {
  size_t cursor_position;
  if (inline_autocomplete_text_.empty()) {
    size_t start;
    view_->GetSelectionBounds(&start, &cursor_position);
    const size_t cursor_offset =
        user_text_.length() - DisplayTextFromUserText(user_text_).length();
    cursor_position += cursor_offset;
  } else {
    cursor_position = user_text_.length();
  }

  GURL current_url =
      (delegate_->CurrentPageExists() && view_->IsIndicatingQueryRefinement()) ?
      delegate_->GetURL() : GURL();
  bool keyword_is_selected = KeywordIsSelected();
  input_ = AutocompleteInput(
      user_text_,
      cursor_position,
      base::string16(),
      current_url,
      ClassifyPage(),
      prevent_inline_autocomplete || just_deleted_text_ ||
      (has_selected_text && inline_autocomplete_text_.empty()) ||
      (paste_state_ != NONE),
      keyword_is_selected,
      keyword_is_selected || allow_exact_keyword_match_,
      true);

  omnibox_controller_->StartAutocomplete(input_);
}
