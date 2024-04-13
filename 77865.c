void RenderFrameImpl::OpenURL(WebFrame* frame,
                              const GURL& url,
                              const Referrer& referrer,
                              WebNavigationPolicy policy) {
  DCHECK_EQ(frame_, frame);

  FrameHostMsg_OpenURL_Params params;
  params.url = url;
  params.referrer = referrer;
  params.disposition = RenderViewImpl::NavigationPolicyToDisposition(policy);
  WebDataSource* ds = frame->provisionalDataSource();
  if (ds) {
    DocumentState* document_state = DocumentState::FromDataSource(ds);
    NavigationState* navigation_state = document_state->navigation_state();
    if (navigation_state->is_content_initiated()) {
      params.should_replace_current_entry = ds->replacesCurrentHistoryItem();
    } else {
      params.should_replace_current_entry =
          navigation_state->should_replace_current_entry();
    }
  } else {
    params.should_replace_current_entry = false;
  }
  params.user_gesture = WebUserGestureIndicator::isProcessingUserGesture();
  if (GetContentClient()->renderer()->AllowPopup())
    params.user_gesture = true;

  if (policy == blink::WebNavigationPolicyNewBackgroundTab ||
      policy == blink::WebNavigationPolicyNewForegroundTab ||
      policy == blink::WebNavigationPolicyNewWindow ||
      policy == blink::WebNavigationPolicyNewPopup) {
    WebUserGestureIndicator::consumeUserGesture();
  }

  Send(new FrameHostMsg_OpenURL(routing_id_, params));
}
