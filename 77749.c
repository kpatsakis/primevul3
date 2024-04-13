void InputMethodBase::OnFocus() {
  DCHECK(!system_toplevel_window_focused_);
  system_toplevel_window_focused_ = true;
}
