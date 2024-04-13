void WebContentsImpl::SwappedOut(RenderViewHost* rvh) {
  if (delegate_ && rvh == GetRenderViewHost())
    delegate_->SwappedOut(this);

  render_manager_.SwappedOut(rvh);
}
