void OmniboxViewWin::OnTemporaryTextMaybeChanged(const string16& display_text,
                                                 bool save_original_selection) {
  if (save_original_selection)
    GetSelection(original_selection_);

  ScopedFreeze freeze(this, GetTextObjectModel());
  SetWindowTextAndCaretPos(display_text, display_text.length());
  TextChanged();
}
