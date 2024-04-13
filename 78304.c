void OmniboxEditModel::OnPopupDataChanged(
    const base::string16& text,
    GURL* destination_for_temporary_text_change,
    const base::string16& keyword,
    bool is_keyword_hint) {
  omnibox_controller_->InvalidateCurrentMatch();

  bool keyword_state_changed = (keyword_ != keyword) ||
      ((is_keyword_hint_ != is_keyword_hint) && !keyword.empty());
  if (keyword_state_changed) {
    keyword_ = keyword;
    is_keyword_hint_ = is_keyword_hint;

    DCHECK(!keyword_.empty() || !is_keyword_hint_);
  }

  if (destination_for_temporary_text_change != NULL) {
    const bool save_original_selection = !has_temporary_text_;
    if (save_original_selection) {
      has_temporary_text_ = true;
      original_url_ = *destination_for_temporary_text_change;
      inline_autocomplete_text_.clear();
      view_->OnInlineAutocompleteTextCleared();
    }
    if (control_key_state_ == DOWN_WITHOUT_CHANGE) {
      control_key_state_ = DOWN_WITH_CHANGE;
    }
    view_->OnTemporaryTextMaybeChanged(DisplayTextFromUserText(text),
                                       save_original_selection, true);
    return;
  }

  bool call_controller_onchanged = true;
  inline_autocomplete_text_ = text;
  if (inline_autocomplete_text_.empty())
    view_->OnInlineAutocompleteTextCleared();

  const base::string16& user_text =
      user_input_in_progress_ ? user_text_ : permanent_text_;
  if (keyword_state_changed && KeywordIsSelected()) {
    view_->SetWindowTextAndCaretPos(DisplayTextFromUserText(user_text), 0,
                                                            false, false);
  } else if (view_->OnInlineAutocompleteTextMaybeChanged(
             DisplayTextFromUserText(user_text + inline_autocomplete_text_),
             DisplayTextFromUserText(user_text).length())) {
    call_controller_onchanged = false;
  }

  if (has_temporary_text_) {
    RevertTemporaryText(false);
    call_controller_onchanged = false;
  }

  if (call_controller_onchanged)
    OnChanged();
}
