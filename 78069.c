void WebContentsImpl::GetRenderViewHostAtPosition(
    int x,
    int y,
    const base::Callback<void(RenderViewHost*, int, int)>& callback) {
  BrowserPluginEmbedder* embedder = GetBrowserPluginEmbedder();
  if (embedder)
    embedder->GetRenderViewHostAtPosition(x, y, callback);
  else
    callback.Run(GetRenderViewHost(), x, y);
}
