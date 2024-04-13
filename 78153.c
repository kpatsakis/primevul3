void WebContentsImpl::RemoveBrowserPluginEmbedder() {
  if (browser_plugin_embedder_)
    browser_plugin_embedder_.reset();
}
