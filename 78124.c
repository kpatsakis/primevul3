void WebContentsImpl::OnBrowserPluginMessage(const IPC::Message& message) {
  CHECK(!browser_plugin_embedder_.get());
  browser_plugin_embedder_.reset(BrowserPluginEmbedder::Create(this));
  browser_plugin_embedder_->OnMessageReceived(message);
}
