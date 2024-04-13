void RenderFrameImpl::OnPasteAndMatchStyle() {
  base::AutoReset<bool> handling_select_range(&handling_select_range_, true);
  frame_->executeCommand(
      WebString::fromUTF8("PasteAndMatchStyle"), GetFocusedElement());
}
