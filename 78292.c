const OmniboxEditModel::State OmniboxEditModel::GetStateForTabSwitch() {
  if (user_input_in_progress_) {
    const base::string16 user_text(UserTextFromDisplayText(view_->GetText()));
    if (user_text.empty()) {
      base::AutoReset<bool> tmp(&in_revert_, true);
      view_->RevertAll();
      view_->SelectAll(true);
    } else {
      InternalSetUserText(user_text);
    }
  }

  UMA_HISTOGRAM_BOOLEAN("Omnibox.SaveStateForTabSwitch.UserInputInProgress",
                        user_input_in_progress_);
  return State(
      user_input_in_progress_, user_text_, view_->GetGrayTextAutocompletion(),
      keyword_, is_keyword_hint_,
      controller_->GetToolbarModel()->url_replacement_enabled(),
      focus_state_, focus_source_, input_);
}
