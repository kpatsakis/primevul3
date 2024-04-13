void WebContentsImpl::HandleGestureEnd() {
  if (delegate_)
    delegate_->HandleGestureEnd();
}
