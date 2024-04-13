void OmniboxEditModel::OnWillKillFocus(gfx::NativeView view_gaining_focus) {
  if (user_input_in_progress_ || !in_revert_)
    delegate_->OnInputStateChanged();
}
