WebApplicationCacheHost* RenderView::createApplicationCacheHost(
    WebFrame* frame, WebApplicationCacheHostClient* client) {
  return new RendererWebApplicationCacheHostImpl(
      FromWebView(frame->view()), client,
      RenderThread::current()->appcache_dispatcher()->backend_proxy());
}
