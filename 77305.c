bool OmniboxViewWin::OnInlineAutocompleteTextMaybeChanged(
    const string16& display_text,
    size_t user_text_length) {
  if (display_text == GetText())
    return false;

  ScopedFreeze freeze(this, GetTextObjectModel());
  SetWindowText(display_text.c_str());
  SetSelection(static_cast<LONG>(display_text.length()),
               static_cast<LONG>(user_text_length));
  TextChanged();
  return true;
}
