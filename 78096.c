void WebContentsImpl::HandleKeyboardEvent(const NativeWebKeyboardEvent& event) {
  if (browser_plugin_embedder_ &&
      browser_plugin_embedder_->HandleKeyboardEvent(event)) {
    return;
  }

  if (delegate_)
    delegate_->HandleKeyboardEvent(this, event);
}
