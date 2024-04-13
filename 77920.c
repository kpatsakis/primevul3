void RenderFrameImpl::didReceiveServerRedirectForProvisionalLoad(
    blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  render_view_->history_controller()->RemoveChildrenForRedirect(this);
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
    Send(new FrameHostMsg_DidRedirectProvisionalLoad(
        routing_id_,
        render_view_->page_id_,
        redirects[redirects.size() - 2],
        redirects.back()));
  }
}
