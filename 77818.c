void RenderFrameImpl::DidCommitCompositorFrame() {
  if (compositing_helper_)
    compositing_helper_->DidCommitCompositorFrame();
}
