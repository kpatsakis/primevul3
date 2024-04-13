void OmniboxViewWin::RevertAll() {
  ScopedFreeze freeze(this, GetTextObjectModel());
  ClosePopup();
  model_->Revert();
  saved_selection_for_focus_change_.cpMin = -1;
  TextChanged();
}
