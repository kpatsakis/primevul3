void WebContentsImpl::SystemDragEnded() {
  if (GetRenderViewHost())
    GetRenderViewHostImpl()->DragSourceSystemDragEnded();
  if (delegate_)
    delegate_->DragEnded();
  if (browser_plugin_embedder_.get())
    browser_plugin_embedder_->SystemDragEnded();
}
