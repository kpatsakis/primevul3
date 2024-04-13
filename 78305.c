void OmniboxEditModel::OnSetFocus(bool control_down) {
  last_omnibox_focus_ = base::TimeTicks::Now();
  user_input_since_focus_ = false;

  SetFocusState(OMNIBOX_FOCUS_VISIBLE, OMNIBOX_FOCUS_CHANGE_EXPLICIT);
  control_key_state_ = control_down ? DOWN_WITHOUT_CHANGE : UP;

  if (delegate_->CurrentPageExists() && !user_input_in_progress_) {
    autocomplete_controller()->StartZeroSuggest(AutocompleteInput(
        permanent_text_, base::string16::npos, base::string16(),
        delegate_->GetURL(), ClassifyPage(), false, false, true, true));
  }

  if (user_input_in_progress_ || !in_revert_)
    delegate_->OnInputStateChanged();
}
