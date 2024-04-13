void RenderFrameImpl::didFailLoad(blink::WebLocalFrame* frame,
                                  const blink::WebURLError& error) {
  DCHECK(!frame_ || frame_ == frame);
  WebDataSource* ds = frame->dataSource();
  DCHECK(ds);

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    DidFailLoad(frame, error));

  const WebURLRequest& failed_request = ds->request();
  base::string16 error_description;
  GetContentClient()->renderer()->GetNavigationErrorStrings(
      render_view_.get(),
      frame,
      failed_request,
      error,
      NULL,
      &error_description);
  Send(new FrameHostMsg_DidFailLoadWithError(routing_id_,
                                             failed_request.url(),
                                             error.reason,
                                             error_description));
}
