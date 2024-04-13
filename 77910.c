void RenderFrameImpl::didFailProvisionalLoad(blink::WebLocalFrame* frame,
                                             const blink::WebURLError& error) {
  DCHECK(!frame_ || frame_ == frame);
  WebDataSource* ds = frame->provisionalDataSource();
  DCHECK(ds);

  const WebURLRequest& failed_request = ds->request();

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    DidFailProvisionalLoad(frame, error));
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_,
                    DidFailProvisionalLoad(error));

  bool show_repost_interstitial =
      (error.reason == net::ERR_CACHE_MISS &&
       EqualsASCII(failed_request.httpMethod(), "POST"));

  FrameHostMsg_DidFailProvisionalLoadWithError_Params params;
  params.frame_unique_name = frame->uniqueName();
  params.error_code = error.reason;
  GetContentClient()->renderer()->GetNavigationErrorStrings(
      render_view_.get(),
      frame,
      failed_request,
      error,
      NULL,
      &params.error_description);
  params.url = error.unreachableURL;
  params.showing_repost_interstitial = show_repost_interstitial;
  Send(new FrameHostMsg_DidFailProvisionalLoadWithError(
      routing_id_, params));

  if (error.reason == net::ERR_ABORTED)
    return;

  if (error.reason == net::ERR_BLOCKED_BY_CLIENT &&
      render_view_->renderer_preferences_.disable_client_blocked_error_page) {
    return;
  }

  if (GetContentClient()->renderer()->ShouldSuppressErrorPage(this,
          error.unreachableURL)) {
    return;
  }

  if (RenderThreadImpl::current() &&
      RenderThreadImpl::current()->layout_test_mode()) {
    return;
  }

  frame->enableViewSourceMode(false);

  DocumentState* document_state = DocumentState::FromDataSource(ds);
  NavigationState* navigation_state = document_state->navigation_state();

  bool replace =
      navigation_state->pending_page_id() != -1 ||
      PageTransitionCoreTypeIs(navigation_state->transition_type(),
                               PAGE_TRANSITION_AUTO_SUBFRAME);

  if (!navigation_state->is_content_initiated()) {
    render_view_->pending_navigation_params_.reset(
        new FrameMsg_Navigate_Params);
    render_view_->pending_navigation_params_->page_id =
        navigation_state->pending_page_id();
    render_view_->pending_navigation_params_->pending_history_list_offset =
        navigation_state->pending_history_list_offset();
    render_view_->pending_navigation_params_->should_clear_history_list =
        navigation_state->history_list_was_cleared();
    render_view_->pending_navigation_params_->transition =
        navigation_state->transition_type();
    render_view_->pending_navigation_params_->request_time =
        document_state->request_time();
    render_view_->pending_navigation_params_->should_replace_current_entry =
        replace;
  }

  LoadNavigationErrorPage(failed_request, error, replace);
}
