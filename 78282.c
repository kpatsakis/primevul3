void OmniboxEditModel::AcceptTemporaryTextAsUserText() {
  InternalSetUserText(UserTextFromDisplayText(view_->GetText()));
  has_temporary_text_ = false;

  if (user_input_in_progress_ || !in_revert_)
    delegate_->OnInputStateChanged();
}
