void RenderFrameImpl::didStopLoading() {
  if (!render_view_->is_loading())
    return;
  render_view_->FrameDidStopLoading(frame_);

  if (!render_view_->is_loading())
    Send(new FrameHostMsg_DidStopLoading(routing_id_));
}
