void OmniboxViewWin::OnBeforePossibleChange() {
  text_before_change_ = GetText();
  GetSelection(sel_before_change_);
}
