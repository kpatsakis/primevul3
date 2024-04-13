void OmniboxViewWin::OnRevertTemporaryText() {
  SetSelectionRange(original_selection_);
  TextChanged();
}
