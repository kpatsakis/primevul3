void RenderView::OnNavigate(const ViewMsg_Navigate_Params& params) {
  if (!webview())
    return;

  history_list_offset_ = params.current_history_list_offset;
  history_list_length_ = params.current_history_list_length;

  if (devtools_agent_.get())
    devtools_agent_->OnNavigate();

  if (notification_provider_.get())
    notification_provider_->OnNavigate();

  child_process_logging::SetActiveURL(params.url);

  AboutHandler::MaybeHandle(params.url);

  bool is_reload =
      params.navigation_type == ViewMsg_Navigate_Params::RELOAD ||
      params.navigation_type == ViewMsg_Navigate_Params::RELOAD_IGNORING_CACHE;

  WebFrame* main_frame = webview()->mainFrame();
  if (is_reload && main_frame->currentHistoryItem().isNull()) {
    is_reload = false;
  }

  if (!params.url.SchemeIs(chrome::kJavaScriptScheme)) {
    NavigationState* state = NavigationState::CreateBrowserInitiated(
        params.page_id,
        params.pending_history_list_offset,
        params.transition,
        params.request_time);
    if (params.navigation_type == ViewMsg_Navigate_Params::RESTORE) {
      state->set_cache_policy_override(WebURLRequest::UseProtocolCachePolicy);
    }
    pending_navigation_state_.reset(state);
  }

  NavigationState* navigation_state = pending_navigation_state_.get();

  if (is_reload) {
    if (navigation_state)
      navigation_state->set_load_type(NavigationState::RELOAD);
    bool ignore_cache = (params.navigation_type ==
                             ViewMsg_Navigate_Params::RELOAD_IGNORING_CACHE);
    main_frame->reload(ignore_cache);
  } else if (!params.state.empty()) {
    DCHECK_NE(params.page_id, -1);
    if (navigation_state)
      navigation_state->set_load_type(NavigationState::HISTORY_LOAD);
    main_frame->loadHistoryItem(
        webkit_glue::HistoryItemFromString(params.state));
  } else {
    WebURLRequest request(params.url);

    DCHECK_EQ(params.page_id, -1);

    if (main_frame->isViewSourceModeEnabled())
      request.setCachePolicy(WebURLRequest::ReturnCacheDataElseLoad);

    if (params.referrer.is_valid()) {
      request.setHTTPHeaderField(WebString::fromUTF8("Referer"),
                                 WebString::fromUTF8(params.referrer.spec()));
    }

    if (navigation_state)
      navigation_state->set_load_type(NavigationState::NORMAL_LOAD);
    main_frame->loadRequest(request);
  }

  pending_navigation_state_.reset();
}
