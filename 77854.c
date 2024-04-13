void RenderFrameImpl::OnPaste() {
  base::AutoReset<bool> handling_select_range(&handling_select_range_, true);
  frame_->executeCommand(WebString::fromUTF8("Paste"), GetFocusedElement());
}
