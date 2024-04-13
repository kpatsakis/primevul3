void RenderView::CheckPreferredSize() {
  if (!send_preferred_size_changes_ || !webview())
    return;

  gfx::Size size(webview()->mainFrame()->contentsPreferredWidth(),
                 webview()->mainFrame()->documentElementScrollHeight());
  if (size == preferred_size_)
    return;

  preferred_size_ = size;
  Send(new ViewHostMsg_DidContentsPreferredSizeChange(routing_id_,
                                                      preferred_size_));
}
