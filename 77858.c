void RenderFrameImpl::OnReplaceMisspelling(const base::string16& text) {
  if (!frame_->hasSelection())
    return;

  frame_->replaceMisspelledRange(text);
}
