void RenderFrameImpl::UpdateURL(blink::WebFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  WebDataSource* ds = frame->dataSource();
  DCHECK(ds);

  const WebURLRequest& request = ds->request();
  const WebURLResponse& response = ds->response();

  DocumentState* document_state = DocumentState::FromDataSource(ds);
  NavigationState* navigation_state = document_state->navigation_state();
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);

  FrameHostMsg_DidCommitProvisionalLoad_Params params;
  params.http_status_code = response.httpStatusCode();
  params.is_post = false;
  params.post_id = -1;
  params.page_id = render_view_->page_id_;
  params.frame_unique_name = frame->uniqueName();
  params.socket_address.set_host(response.remoteIPAddress().utf8());
  params.socket_address.set_port(response.remotePort());
  WebURLResponseExtraDataImpl* extra_data = GetExtraDataFromResponse(response);
  if (extra_data)
    params.was_fetched_via_proxy = extra_data->was_fetched_via_proxy();
  params.was_within_same_page = navigation_state->was_within_same_page();
  params.security_info = response.securityInfo();

  params.url = render_view_->GetLoadingUrl(frame);
  DCHECK(!is_swapped_out_ || params.url == GURL(kSwappedOutURL));

  if (frame->document().baseURL() != params.url)
    params.base_url = frame->document().baseURL();

  GetRedirectChain(ds, &params.redirects);
  params.should_update_history = !ds->hasUnreachableURL() &&
      !response.isMultipartPayload() && (response.httpStatusCode() != 404);

  params.searchable_form_url = internal_data->searchable_form_url();
  params.searchable_form_encoding = internal_data->searchable_form_encoding();

  params.gesture = render_view_->navigation_gesture_;
  render_view_->navigation_gesture_ = NavigationGestureUnknown;

  HistoryEntry* entry = render_view_->history_controller()->GetCurrentEntry();
  if (entry)
    params.page_state = HistoryEntryToPageState(entry);
  else
    params.page_state = PageState::CreateFromURL(request.url());

  if (!frame->parent()) {

    render_view_->webview()->zoomLimitsChanged(
        ZoomFactorToZoomLevel(kMinimumZoomFactor),
        ZoomFactorToZoomLevel(kMaximumZoomFactor));

    HostZoomLevels::iterator host_zoom =
        render_view_->host_zoom_levels_.find(GURL(request.url()));
    if (render_view_->webview()->mainFrame()->document().isPluginDocument()) {
      render_view_->webview()->setZoomLevel(0);
    } else {
      if (host_zoom != render_view_->host_zoom_levels_.end())
        render_view_->webview()->setZoomLevel(host_zoom->second);
    }

    if (host_zoom != render_view_->host_zoom_levels_.end()) {
      render_view_->host_zoom_levels_.erase(host_zoom);
    }

    params.contents_mime_type = ds->response().mimeType().utf8();

    params.transition = navigation_state->transition_type();
    if (!PageTransitionIsMainFrame(params.transition)) {
      params.transition = PAGE_TRANSITION_LINK;
    }

    if (ds->isClientRedirect()) {
      params.referrer =
          Referrer(params.redirects[0], ds->request().referrerPolicy());
      params.transition = static_cast<PageTransition>(
          params.transition | PAGE_TRANSITION_CLIENT_REDIRECT);
    } else {
      params.referrer = RenderViewImpl::GetReferrerFromRequest(
          frame, ds->request());
    }

    base::string16 method = request.httpMethod();
    if (EqualsASCII(method, "POST")) {
      params.is_post = true;
      params.post_id = ExtractPostId(entry->root());
    }

    params.is_overriding_user_agent = internal_data->is_overriding_user_agent();

    params.original_request_url = GetOriginalRequestURL(ds);

    params.history_list_was_cleared =
        navigation_state->history_list_was_cleared();

    UMA_HISTOGRAM_COUNTS_10000("Memory.GlyphPagesPerLoad",
                               blink::WebGlyphCache::pageCount());

    Send(new FrameHostMsg_DidCommitProvisionalLoad(routing_id_, params));
  } else {
    if (render_view_->page_id_ > render_view_->last_page_id_sent_to_browser_)
      params.transition = PAGE_TRANSITION_MANUAL_SUBFRAME;
    else
      params.transition = PAGE_TRANSITION_AUTO_SUBFRAME;

    DCHECK(!navigation_state->history_list_was_cleared());
    params.history_list_was_cleared = false;

    if (!is_swapped_out())
      Send(new FrameHostMsg_DidCommitProvisionalLoad(routing_id_, params));
  }

  render_view_->last_page_id_sent_to_browser_ =
      std::max(render_view_->last_page_id_sent_to_browser_,
               render_view_->page_id_);

  navigation_state->set_transition_type(PAGE_TRANSITION_LINK);
}
