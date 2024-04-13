void RenderFrameImpl::OnReplace(const base::string16& text) {
  if (!frame_->hasSelection())
    frame_->selectWordAroundCaret();

  frame_->replaceSelection(text);
}
