void WebContentsImpl::HandleGestureBegin() {
  if (delegate_)
    delegate_->HandleGestureBegin();
}
