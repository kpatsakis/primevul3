void OmniboxViewWin::SetUserText(const string16& text,
                                 const string16& display_text,
                                 bool update_popup) {
  ScopedFreeze freeze(this, GetTextObjectModel());
  model_->SetUserText(text);
  saved_selection_for_focus_change_.cpMin = -1;
  SetWindowTextAndCaretPos(display_text, display_text.length());
  if (update_popup)
    UpdatePopup();
  TextChanged();
}
