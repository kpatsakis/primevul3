void OmniboxViewWin::OnPaste() {
  const string16 text(GetClipboardText());
  if (!text.empty()) {
    model_->on_paste();
    text_before_change_.clear();
    ReplaceSel(text.c_str(), true);
  }
}
