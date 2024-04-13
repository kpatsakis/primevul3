void RenderFrameImpl::didStartProvisionalLoad(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  WebDataSource* ds = frame->provisionalDataSource();

  if (!ds)
    return;

  DocumentState* document_state = DocumentState::FromDataSource(ds);

  CHECK((ds->request().url() != GURL(kSwappedOutURL)) ||
        is_swapped_out_ ||
        render_view_->is_swapped_out()) <<
        "Heard swappedout:// when not swapped out.";

  if (document_state->request_time().is_null()) {
    double event_time = ds->triggeringEventTime();
    if (event_time != 0.0)
      document_state->set_request_time(Time::FromDoubleT(event_time));
  }

  document_state->set_start_load_time(Time::Now());

  bool is_top_most = !frame->parent();
  if (is_top_most) {
    render_view_->set_navigation_gesture(
        WebUserGestureIndicator::isProcessingUserGesture() ?
            NavigationGestureUser : NavigationGestureAuto);
  } else if (ds->replacesCurrentHistoryItem()) {
    document_state->navigation_state()->set_transition_type(
        PAGE_TRANSITION_AUTO_SUBFRAME);
  }

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    DidStartProvisionalLoad(frame));
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, DidStartProvisionalLoad());

  int parent_routing_id = frame->parent() ?
      FromWebFrame(frame->parent())->GetRoutingID() : -1;
  Send(new FrameHostMsg_DidStartProvisionalLoadForFrame(
       routing_id_, parent_routing_id, ds->request().url()));
}
