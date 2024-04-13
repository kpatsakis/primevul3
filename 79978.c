void RenderViewImpl::DidFlushPaint() {
  pepper_helper_->ViewFlushedPaint();

  if (!webview())
    return;

  WebFrame* main_frame = webview()->mainFrame();

  if (!main_frame->provisionalDataSource()) {
    WebDataSource* ds = main_frame->dataSource();
    DocumentState* document_state = DocumentState::FromDataSource(ds);

    Time now = Time::Now();
    if (document_state->first_paint_time().is_null()) {
      document_state->set_first_paint_time(now);
    }
    if (document_state->first_paint_after_load_time().is_null() &&
        !document_state->finish_load_time().is_null()) {
      document_state->set_first_paint_after_load_time(now);
    }
  }
}
