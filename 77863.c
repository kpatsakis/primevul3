void RenderFrameImpl::OnUndo() {
  frame_->executeCommand(WebString::fromUTF8("Undo"), GetFocusedElement());
}
