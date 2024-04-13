blink::WebApplicationCacheHost* RenderFrameImpl::createApplicationCacheHost(
    blink::WebLocalFrame* frame,
    blink::WebApplicationCacheHostClient* client) {
  if (!frame || !frame->view())
    return NULL;
  DCHECK(!frame_ || frame_ == frame);
  return new RendererWebApplicationCacheHostImpl(
      RenderViewImpl::FromWebView(frame->view()), client,
      RenderThreadImpl::current()->appcache_dispatcher()->backend_proxy());
}
