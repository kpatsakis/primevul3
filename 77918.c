void RenderFrameImpl::didNavigateWithinPage(blink::WebLocalFrame* frame,
    const blink::WebHistoryItem& item,
    blink::WebHistoryCommitType commit_type) {
  DCHECK(!frame_ || frame_ == frame);
  didCreateDataSource(frame, frame->dataSource());

  DocumentState* document_state =
      DocumentState::FromDataSource(frame->dataSource());
  NavigationState* new_state = document_state->navigation_state();
  new_state->set_was_within_same_page(true);

  didCommitProvisionalLoad(frame, item, commit_type);
}
