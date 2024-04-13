void RenderView::didReceiveServerRedirectForProvisionalLoad(WebFrame* frame) {
  if (frame->parent())
    return;
  WebDataSource* data_source = frame->provisionalDataSource();
  if (!data_source) {
    NOTREACHED();
    return;
  }
  std::vector<GURL> redirects;
  GetRedirectChain(data_source, &redirects);
  if (redirects.size() >= 2) {
    Send(new ViewHostMsg_DidRedirectProvisionalLoad(routing_id_, page_id_,
        redirects[redirects.size() - 2], redirects.back()));
  }
}
