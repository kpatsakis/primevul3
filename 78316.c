void OmniboxEditModel::SetUserText(const base::string16& text) {
  SetInputInProgress(true);
  InternalSetUserText(text);
  omnibox_controller_->InvalidateCurrentMatch();
  paste_state_ = NONE;
  has_temporary_text_ = false;
}
