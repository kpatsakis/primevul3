void OmniboxEditModel::OnPaste() {
  UMA_HISTOGRAM_COUNTS("Omnibox.Paste", 1);
  paste_state_ = PASTING;
}
