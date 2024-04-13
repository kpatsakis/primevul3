void RenderFrameImpl::willSendRequest(
    blink::WebLocalFrame* frame,
    unsigned identifier,
    blink::WebURLRequest& request,
    const blink::WebURLResponse& redirect_response) {
  DCHECK(!frame_ || frame_ == frame);
  if (request.url().isEmpty())
    return;

  WebFrame* top_frame = frame->top();
  if (!top_frame)
    top_frame = frame;
  WebDataSource* provisional_data_source = top_frame->provisionalDataSource();
  WebDataSource* top_data_source = top_frame->dataSource();
  WebDataSource* data_source =
      provisional_data_source ? provisional_data_source : top_data_source;

  PageTransition transition_type = PAGE_TRANSITION_LINK;
  DocumentState* document_state = DocumentState::FromDataSource(data_source);
  DCHECK(document_state);
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);
  NavigationState* navigation_state = document_state->navigation_state();
  transition_type = navigation_state->transition_type();

  GURL request_url(request.url());
  GURL new_url;
  if (GetContentClient()->renderer()->WillSendRequest(
          frame,
          transition_type,
          request_url,
          request.firstPartyForCookies(),
          &new_url)) {
    request.setURL(WebURL(new_url));
  }

  if (internal_data->is_cache_policy_override_set())
    request.setCachePolicy(internal_data->cache_policy_override());

  WebString custom_user_agent;
  bool was_after_preconnect_request = false;
  if (request.extraData()) {
    RequestExtraData* old_extra_data =
        static_cast<RequestExtraData*>(
            request.extraData());
    custom_user_agent = old_extra_data->custom_user_agent();
    was_after_preconnect_request =
        old_extra_data->was_after_preconnect_request();

    if (!custom_user_agent.isNull()) {
      if (custom_user_agent.isEmpty())
        request.clearHTTPHeaderField("User-Agent");
      else
        request.setHTTPHeaderField("User-Agent", custom_user_agent);
    }
  }

  bool should_replace_current_entry = false;
  if (navigation_state->is_content_initiated()) {
    should_replace_current_entry = data_source->replacesCurrentHistoryItem();
  } else {
    should_replace_current_entry =
        navigation_state->should_replace_current_entry();
  }

  int provider_id = kInvalidServiceWorkerProviderId;
  if (request.targetType() == blink::WebURLRequest::TargetIsMainFrame ||
      request.targetType() == blink::WebURLRequest::TargetIsSubframe) {
    if (frame->provisionalDataSource()) {
      ServiceWorkerNetworkProvider* provider =
          ServiceWorkerNetworkProvider::FromDocumentState(
              DocumentState::FromDataSource(frame->provisionalDataSource()));
      provider_id = provider->provider_id();
    }
  } else if (frame->dataSource()) {
    ServiceWorkerNetworkProvider* provider =
        ServiceWorkerNetworkProvider::FromDocumentState(
            DocumentState::FromDataSource(frame->dataSource()));
    provider_id = provider->provider_id();
  }

  int parent_routing_id = frame->parent() ?
      FromWebFrame(frame->parent())->GetRoutingID() : -1;
  RequestExtraData* extra_data = new RequestExtraData();
  extra_data->set_visibility_state(render_view_->visibilityState());
  extra_data->set_custom_user_agent(custom_user_agent);
  extra_data->set_was_after_preconnect_request(was_after_preconnect_request);
  extra_data->set_render_frame_id(routing_id_);
  extra_data->set_is_main_frame(frame == top_frame);
  extra_data->set_frame_origin(
      GURL(frame->document().securityOrigin().toString()));
  extra_data->set_parent_is_main_frame(frame->parent() == top_frame);
  extra_data->set_parent_render_frame_id(parent_routing_id);
  extra_data->set_allow_download(navigation_state->allow_download());
  extra_data->set_transition_type(transition_type);
  extra_data->set_should_replace_current_entry(should_replace_current_entry);
  extra_data->set_transferred_request_child_id(
      navigation_state->transferred_request_child_id());
  extra_data->set_transferred_request_request_id(
      navigation_state->transferred_request_request_id());
  extra_data->set_service_worker_provider_id(provider_id);
  request.setExtraData(extra_data);

  DocumentState* top_document_state =
      DocumentState::FromDataSource(top_data_source);
  if (top_document_state) {
    if (request.targetType() == WebURLRequest::TargetIsPrefetch)
      top_document_state->set_was_prefetcher(true);

    if (was_after_preconnect_request)
      top_document_state->set_was_after_preconnect_request(true);
  }

  request.setRequestorID(render_view_->GetRoutingID());
  request.setHasUserGesture(WebUserGestureIndicator::isProcessingUserGesture());

  if (!navigation_state->extra_headers().empty()) {
    for (net::HttpUtil::HeadersIterator i(
        navigation_state->extra_headers().begin(),
        navigation_state->extra_headers().end(), "\n");
        i.GetNext(); ) {
      if (LowerCaseEqualsASCII(i.name(), "referer")) {
        WebString referrer = WebSecurityPolicy::generateReferrerHeader(
            blink::WebReferrerPolicyDefault,
            request.url(),
            WebString::fromUTF8(i.values()));
        request.setHTTPReferrer(referrer, blink::WebReferrerPolicyDefault);
      } else {
        request.setHTTPHeaderField(WebString::fromUTF8(i.name()),
                                   WebString::fromUTF8(i.values()));
      }
    }
  }

  if (!render_view_->renderer_preferences_.enable_referrers)
    request.setHTTPReferrer(WebString(), blink::WebReferrerPolicyDefault);
}
