void RenderView::didCommitProvisionalLoad(WebFrame* frame,
                                          bool is_new_navigation) {
  NavigationState* navigation_state =
      NavigationState::FromDataSource(frame->dataSource());

  navigation_state->set_commit_load_time(Time::Now());
  if (is_new_navigation) {
    UpdateSessionHistory(frame);

    page_id_ = next_page_id_++;

    translate_helper_.CancelPendingTranslation();

    history_list_offset_++;
    if (history_list_offset_ >= chrome::kMaxSessionHistoryEntries)
      history_list_offset_ = chrome::kMaxSessionHistoryEntries - 1;
    history_list_length_ = history_list_offset_ + 1;

    MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        method_factory_.NewRunnableMethod(&RenderView::CapturePageInfo,
                                          page_id_, true),
        kDelayForForcedCaptureMs);
  } else {
    if (navigation_state->pending_page_id() != -1 &&
        navigation_state->pending_page_id() != page_id_ &&
        !navigation_state->request_committed()) {
      UpdateSessionHistory(frame);
      page_id_ = navigation_state->pending_page_id();

      history_list_offset_ = navigation_state->pending_history_list_offset();
    }
  }

  navigation_state->set_request_committed(true);

  UpdateURL(frame);

  completed_client_redirect_src_ = GURL();

  UpdateEncoding(frame, frame->view()->pageEncoding().utf8());
}
