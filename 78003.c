WebUIImpl* WebContentsImpl::CreateWebUIForRenderManager(const GURL& url) {
  return static_cast<WebUIImpl*>(CreateWebUI(url));
}
