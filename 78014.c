void WebContentsImpl::DidDisownOpener(RenderViewHost* rvh) {
  if (opener_) {
    RemoveDestructionObserver(opener_);
    opener_ = NULL;
  }

  render_manager_.DidDisownOpener(rvh);
}
