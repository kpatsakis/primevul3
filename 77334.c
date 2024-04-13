void OmniboxViewWin::EditDropTarget::ResetDropHighlights() {
  if (drag_has_string_)
    edit_->SetDropHighlightPosition(-1);
}
