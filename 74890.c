WebSharedWorker* RenderView::createSharedWorker(
    WebFrame* frame, const WebURL& url, const WebString& name,
    unsigned long long document_id) {

  int route_id = MSG_ROUTING_NONE;
  bool exists = false;
  bool url_mismatch = false;
  ViewHostMsg_CreateWorker_Params params;
  params.url = url;
  params.is_shared = true;
  params.name = name;
  params.document_id = document_id;
  params.render_view_route_id = routing_id_;
  params.route_id = MSG_ROUTING_NONE;
  params.parent_appcache_host_id = 0;
  params.script_resource_appcache_id = 0;
  Send(new ViewHostMsg_LookupSharedWorker(
      params, &exists, &route_id, &url_mismatch));
  if (url_mismatch) {
    return NULL;
  } else {
    return new WebSharedWorkerProxy(RenderThread::current(),
                                    document_id,
                                    exists,
                                    route_id,
                                    routing_id_);
  }
}
