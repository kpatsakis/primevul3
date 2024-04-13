void RenderFrameImpl::OnCopy() {
  base::AutoReset<bool> handling_select_range(&handling_select_range_, true);
  WebNode current_node = context_menu_node_.isNull() ?
      GetFocusedElement() : context_menu_node_;
  frame_->executeCommand(WebString::fromUTF8("Copy"), current_node);
}
