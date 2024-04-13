void RenderFrameImpl::OnNavigate(const FrameMsg_Navigate_Params& params) {
  MaybeHandleDebugURL(params.url);
  if (!render_view_->webview())
    return;

  render_view_->OnNavigate(params);

  bool is_reload = RenderViewImpl::IsReload(params);
  WebURLRequest::CachePolicy cache_policy =
      WebURLRequest::UseProtocolCachePolicy;

  if (render_view_->IsBackForwardToStaleEntry(params, is_reload))
    return;

  if (render_view_->is_swapped_out_) {
    render_view_->webview()->setVisibilityState(
        render_view_->visibilityState(), false);

    is_reload = false;
    cache_policy = WebURLRequest::ReloadIgnoringCacheData;

    RenderThreadImpl::NotifyTimezoneChange();

    render_view_->SetSwappedOut(false);
    is_swapped_out_ = false;
  }

  if (params.should_clear_history_list) {
    CHECK_EQ(params.pending_history_list_offset, -1);
    CHECK_EQ(params.current_history_list_offset, -1);
    CHECK_EQ(params.current_history_list_length, 0);
  }
  render_view_->history_list_offset_ = params.current_history_list_offset;
  render_view_->history_list_length_ = params.current_history_list_length;
  if (render_view_->history_list_length_ >= 0) {
    render_view_->history_page_ids_.resize(
        render_view_->history_list_length_, -1);
  }
  if (params.pending_history_list_offset >= 0 &&
      params.pending_history_list_offset < render_view_->history_list_length_) {
    render_view_->history_page_ids_[params.pending_history_list_offset] =
        params.page_id;
  }

  GetContentClient()->SetActiveURL(params.url);

  WebFrame* frame = frame_;
  if (!params.frame_to_navigate.empty()) {
    frame = render_view_->webview()->findFrameByName(
        WebString::fromUTF8(params.frame_to_navigate));
    CHECK(frame) << "Invalid frame name passed: " << params.frame_to_navigate;
  }

  if (is_reload && !render_view_->history_controller()->GetCurrentEntry()) {
    is_reload = false;
    cache_policy = WebURLRequest::ReloadIgnoringCacheData;
  }

  render_view_->pending_navigation_params_.reset(
      new FrameMsg_Navigate_Params(params));

  if (is_reload) {
    bool reload_original_url =
        (params.navigation_type ==
            FrameMsg_Navigate_Type::RELOAD_ORIGINAL_REQUEST_URL);
    bool ignore_cache = (params.navigation_type ==
                             FrameMsg_Navigate_Type::RELOAD_IGNORING_CACHE);

    if (reload_original_url)
      frame->reloadWithOverrideURL(params.url, true);
    else
      frame->reload(ignore_cache);
  } else if (params.page_state.IsValid()) {
    DCHECK_NE(params.page_id, -1);
    scoped_ptr<HistoryEntry> entry =
        PageStateToHistoryEntry(params.page_state);
    if (entry) {
      CHECK(entry->root().urlString() != WebString::fromUTF8(kSwappedOutURL));
      render_view_->history_controller()->GoToEntry(entry.Pass(), cache_policy);
    }
  } else if (!params.base_url_for_data_url.is_empty()) {
    std::string mime_type, charset, data;
    if (net::DataURL::Parse(params.url, &mime_type, &charset, &data)) {
      frame->loadData(
          WebData(data.c_str(), data.length()),
          WebString::fromUTF8(mime_type),
          WebString::fromUTF8(charset),
          params.base_url_for_data_url,
          params.history_url_for_data_url,
          false);
    } else {
      CHECK(false) <<
          "Invalid URL passed: " << params.url.possibly_invalid_spec();
    }
  } else {
    WebURLRequest request(params.url);

    CHECK_EQ(params.page_id, -1);

    if (frame->isViewSourceModeEnabled())
      request.setCachePolicy(WebURLRequest::ReturnCacheDataElseLoad);

    if (params.referrer.url.is_valid()) {
      WebString referrer = WebSecurityPolicy::generateReferrerHeader(
          params.referrer.policy,
          params.url,
          WebString::fromUTF8(params.referrer.url.spec()));
      if (!referrer.isEmpty())
        request.setHTTPReferrer(referrer, params.referrer.policy);
    }

    if (!params.extra_headers.empty()) {
      for (net::HttpUtil::HeadersIterator i(params.extra_headers.begin(),
                                            params.extra_headers.end(), "\n");
           i.GetNext(); ) {
        request.addHTTPHeaderField(WebString::fromUTF8(i.name()),
                                   WebString::fromUTF8(i.values()));
      }
    }

    if (params.is_post) {
      request.setHTTPMethod(WebString::fromUTF8("POST"));

      WebHTTPBody http_body;
      http_body.initialize();
      const char* data = NULL;
      if (params.browser_initiated_post_data.size()) {
        data = reinterpret_cast<const char*>(
            &params.browser_initiated_post_data.front());
      }
      http_body.appendData(
          WebData(data, params.browser_initiated_post_data.size()));
      request.setHTTPBody(http_body);
    }

    frame->loadRequest(request);

    if (!params.browser_navigation_start.is_null() &&
        frame->provisionalDataSource()) {
      base::TimeTicks navigation_start = std::min(
          base::TimeTicks::Now(), params.browser_navigation_start);
      double navigation_start_seconds =
          (navigation_start - base::TimeTicks()).InSecondsF();
      frame->provisionalDataSource()->setNavigationStartTime(
          navigation_start_seconds);
    }
  }

  render_view_->pending_navigation_params_.reset();
}
