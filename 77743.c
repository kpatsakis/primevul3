TextInputClient* InputMethodBase::GetTextInputClient() const {
  return system_toplevel_window_focused_ ? text_input_client_ : NULL;
}
