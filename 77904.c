void RenderFrameImpl::didCommitProvisionalLoad(
    blink::WebLocalFrame* frame,
    const blink::WebHistoryItem& item,
    blink::WebHistoryCommitType commit_type) {
  DCHECK(!frame_ || frame_ == frame);
  DocumentState* document_state =
      DocumentState::FromDataSource(frame->dataSource());
  NavigationState* navigation_state = document_state->navigation_state();
  render_view_->history_controller()->UpdateForCommit(this, item, commit_type,
      navigation_state->was_within_same_page());

  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);

  if (document_state->commit_load_time().is_null())
    document_state->set_commit_load_time(Time::Now());

  if (internal_data->must_reset_scroll_and_scale_state()) {
    render_view_->webview()->resetScrollAndScaleState();
    internal_data->set_must_reset_scroll_and_scale_state(false);
  }
  internal_data->set_use_error_page(false);

  bool is_new_navigation = commit_type == blink::WebStandardCommit;
  if (is_new_navigation) {
    render_view_->UpdateSessionHistory(frame);

    render_view_->page_id_ = render_view_->next_page_id_++;

    if (render_view_->GetLoadingUrl(frame) != GURL(kSwappedOutURL)) {
      render_view_->history_list_offset_++;
      if (render_view_->history_list_offset_ >= kMaxSessionHistoryEntries)
        render_view_->history_list_offset_ = kMaxSessionHistoryEntries - 1;
      render_view_->history_list_length_ =
          render_view_->history_list_offset_ + 1;
      render_view_->history_page_ids_.resize(
          render_view_->history_list_length_, -1);
      render_view_->history_page_ids_[render_view_->history_list_offset_] =
          render_view_->page_id_;
    }
  } else {
    if (navigation_state->pending_page_id() != -1 &&
        navigation_state->pending_page_id() != render_view_->page_id_ &&
        !navigation_state->request_committed()) {
      render_view_->UpdateSessionHistory(frame);
      render_view_->page_id_ = navigation_state->pending_page_id();

      render_view_->history_list_offset_ =
          navigation_state->pending_history_list_offset();

      DCHECK(render_view_->history_list_length_ <= 0 ||
             render_view_->history_list_offset_ < 0 ||
             render_view_->history_list_offset_ >=
                 render_view_->history_list_length_ ||
             render_view_->history_page_ids_[render_view_->history_list_offset_]
                  == render_view_->page_id_);
    }
  }

  render_view_->FrameDidCommitProvisionalLoad(frame, is_new_navigation);
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_,
                    DidCommitProvisionalLoad(is_new_navigation));

  navigation_state->set_request_committed(true);

  UpdateURL(frame);

  render_view_->UpdateEncoding(frame, frame->view()->pageEncoding().utf8());
}
