bool InputMethodBase::IsTextInputClientFocused(const TextInputClient* client) {
  return client && (client == GetTextInputClient());
}
