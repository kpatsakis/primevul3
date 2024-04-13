bool OmniboxViewWin::CanPasteAndGo(const string16& text) const {
  return !popup_window_mode_ && model_->CanPasteAndGo(text);
}
