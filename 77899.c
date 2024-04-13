void RenderFrameImpl::didChangeLoadProgress(double load_progress) {
  render_view_->FrameDidChangeLoadProgress(frame_, load_progress);
}
