RenderProcessHost* WebContentsImpl::GetRenderProcessHost() const {
  RenderViewHostImpl* host = render_manager_.current_host();
  return host ? host->GetProcess() : NULL;
}
