void WebContentsImpl::HandleMouseDown() {
  if (delegate_)
    delegate_->HandleMouseDown();
}
