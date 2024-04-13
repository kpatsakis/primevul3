void RenderFrameImpl::OnChildFrameProcessGone() {
  if (compositing_helper_)
    compositing_helper_->ChildFrameGone();
}
