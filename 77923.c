void RenderFrameImpl::didStartLoading(bool to_different_document) {
  bool view_was_loading = render_view_->is_loading();
  render_view_->FrameDidStartLoading(frame_);
  if (!view_was_loading)
    Send(new FrameHostMsg_DidStartLoading(routing_id_, to_different_document));
}
