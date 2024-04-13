void RenderView::UpdateURL(WebFrame* frame) {
  WebDataSource* ds = frame->dataSource();
  DCHECK(ds);

  const WebURLRequest& request = ds->request();
  const WebURLRequest& original_request = ds->originalRequest();
  const WebURLResponse& response = ds->response();

  NavigationState* navigation_state = NavigationState::FromDataSource(ds);
  DCHECK(navigation_state);

  ViewHostMsg_FrameNavigate_Params params;
  params.http_status_code = response.httpStatusCode();
  params.is_post = false;
  params.page_id = page_id_;
  params.is_content_filtered = response.isContentFiltered();
  if (!navigation_state->security_info().empty()) {
    DCHECK(response.securityInfo().isEmpty());
    params.security_info = navigation_state->security_info();
  } else {
    params.security_info = response.securityInfo();
  }

  if (ds->hasUnreachableURL()) {
    params.url = ds->unreachableURL();
  } else {
    params.url = request.url();
  }

  GetRedirectChain(ds, &params.redirects);
  params.should_update_history = !ds->hasUnreachableURL() &&
      !response.isMultipartPayload();

  params.searchable_form_url = navigation_state->searchable_form_url();
  params.searchable_form_encoding =
      navigation_state->searchable_form_encoding();

  const PasswordForm* password_form_data =
      navigation_state->password_form_data();
  if (password_form_data)
    params.password_form = *password_form_data;

  params.gesture = navigation_gesture_;
  navigation_gesture_ = NavigationGestureUnknown;

  if (!frame->parent()) {

    ClearBlockedContentSettings();

    HostContentSettings::iterator host_content_settings =
        host_content_settings_.find(GURL(request.url()));
    if (host_content_settings != host_content_settings_.end()) {
      SetContentSettings(host_content_settings->second);

      host_content_settings_.erase(host_content_settings);
    } else if (frame->opener()) {
      if (WebView* opener_view = frame->opener()->view()) {
        RenderView* opener = FromWebView(opener_view);
        SetContentSettings(opener->current_content_settings_);
      }
    }

    HostZoomLevels::iterator host_zoom =
        host_zoom_levels_.find(GURL(request.url()));
    if (host_zoom != host_zoom_levels_.end()) {
      webview()->setZoomLevel(false, host_zoom->second);
      host_zoom_levels_.erase(host_zoom);
    }

    params.contents_mime_type = ds->response().mimeType().utf8();

    params.transition = navigation_state->transition_type();
    if (!PageTransition::IsMainFrame(params.transition)) {
      params.transition = PageTransition::LINK;
    }

    if (completed_client_redirect_src_.is_valid()) {
      DCHECK(completed_client_redirect_src_ == params.redirects[0]);
      params.referrer = completed_client_redirect_src_;
      params.transition = static_cast<PageTransition::Type>(
          params.transition | PageTransition::CLIENT_REDIRECT);
    } else {
      params.referrer = GURL(
          original_request.httpHeaderField(WebString::fromUTF8("Referer")));
    }

    string16 method = request.httpMethod();
    if (EqualsASCII(method, "POST"))
      params.is_post = true;

    UMA_HISTOGRAM_COUNTS_10000("Memory.GlyphPagesPerLoad",
                               webkit_glue::GetGlyphPageCount());

    Send(new ViewHostMsg_FrameNavigate(routing_id_, params));
  } else {
    if (page_id_ > last_page_id_sent_to_browser_)
      params.transition = PageTransition::MANUAL_SUBFRAME;
    else
      params.transition = PageTransition::AUTO_SUBFRAME;

    Send(new ViewHostMsg_FrameNavigate(routing_id_, params));
  }

  last_page_id_sent_to_browser_ =
      std::max(last_page_id_sent_to_browser_, page_id_);

  navigation_state->set_transition_type(PageTransition::LINK);

  if (accessibility_.get()) {
    accessibility_->clear();
    accessibility_.reset();
  }
}
