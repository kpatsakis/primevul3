WebWorker* RenderView::createWorker(WebFrame* frame, WebWorkerClient* client) {
  WebApplicationCacheHostImpl* appcache_host =
      WebApplicationCacheHostImpl::FromFrame(frame);
  int appcache_host_id = appcache_host ? appcache_host->host_id() : 0;
  return new WebWorkerProxy(client, RenderThread::current(), routing_id_,
                            appcache_host_id);
}
