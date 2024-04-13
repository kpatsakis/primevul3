void WebContentsImpl::ShowContextMenu(const ContextMenuParams& params) {
  if (delegate_ && delegate_->HandleContextMenu(params))
    return;

  render_view_host_delegate_view_->ShowContextMenu(params);
}
