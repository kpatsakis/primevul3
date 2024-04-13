void WebContentsImpl::RendererUnresponsive(RenderViewHost* rvh,
                                           bool is_during_beforeunload,
                                           bool is_during_unload) {
  if (rvh != GetRenderViewHost())
    return;

  RenderViewHostImpl* rvhi = static_cast<RenderViewHostImpl*>(rvh);

  if (DevToolsAgentHost::IsDebuggerAttached(this))
    return;

  if (is_during_beforeunload || is_during_unload) {
    rvhi->set_sudden_termination_allowed(true);

    if (!render_manager_.ShouldCloseTabOnUnresponsiveRenderer())
      return;

    bool close = true;
    if (is_during_beforeunload) {
      delegate_->BeforeUnloadFired(this, true, &close);
    }
    if (close)
      Close(rvh);
    return;
  }

  if (!GetRenderViewHostImpl() || !GetRenderViewHostImpl()->IsRenderViewLive())
    return;

  if (delegate_)
    delegate_->RendererUnresponsive(this);
}
