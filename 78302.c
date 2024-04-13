void OmniboxEditModel::OnKillFocus() {
  SetFocusState(OMNIBOX_FOCUS_NONE, OMNIBOX_FOCUS_CHANGE_EXPLICIT);
  focus_source_ = INVALID;
  control_key_state_ = UP;
  paste_state_ = NONE;
}
