void OmniboxViewWin::PasteAndGo(const string16& text) {
  if (CanPasteAndGo(text))
    model_->PasteAndGo();
}
