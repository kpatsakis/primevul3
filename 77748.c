void InputMethodBase::OnBlur() {
  DCHECK(system_toplevel_window_focused_);
  system_toplevel_window_focused_ = false;
}
