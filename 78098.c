void WebContentsImpl::HandleMouseUp() {
  if (delegate_)
    delegate_->HandleMouseUp();
}
