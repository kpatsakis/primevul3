void RenderView::didCreateDataSource(WebFrame* frame, WebDataSource* ds) {
  bool content_initiated = !pending_navigation_state_.get();
  NavigationState* state = content_initiated ?
      NavigationState::CreateContentInitiated() :
      pending_navigation_state_.release();

  const WebURLRequest& original_request = ds->originalRequest();
  const WebURLRequest& request = ds->request();

  if (webview()) {
    if( WebFrame* old_frame = webview()->mainFrame()) {
      const GURL referrer(
          original_request.httpHeaderField(WebString::fromUTF8("Referer")));
      if (!referrer.is_empty() &&
          NavigationState::FromDataSource(
              old_frame->dataSource())->was_prefetcher()) {
        for (;old_frame;old_frame = old_frame->traverseNext(false)) {
          WebDataSource* old_frame_ds = old_frame->dataSource();
          if (old_frame_ds && referrer == GURL(old_frame_ds->request().url())) {
            state->set_was_referred_by_prefetcher(true);
            break;
          }
        }
      }
    }
  }

  if (content_initiated) {
    switch (request.cachePolicy()) {
      case WebURLRequest::UseProtocolCachePolicy:  // normal load.
        state->set_load_type(NavigationState::LINK_LOAD_NORMAL);
        break;
      case WebURLRequest::ReloadIgnoringCacheData:  // reload.
        state->set_load_type(NavigationState::LINK_LOAD_RELOAD);
        break;
      case WebURLRequest::ReturnCacheDataElseLoad:  // allow stale data.
        state->set_load_type(NavigationState::LINK_LOAD_CACHE_STALE_OK);
        break;
      case WebURLRequest::ReturnCacheDataDontLoad:  // Don't re-post.
        state->set_load_type(NavigationState::LINK_LOAD_CACHE_ONLY);
        break;
    }
  }

  state->set_user_script_idle_scheduler(
      new UserScriptIdleScheduler(this, frame));
  ds->setExtraData(state);
}
