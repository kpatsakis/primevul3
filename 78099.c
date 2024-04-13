void WebContentsImpl::HandlePointerActivate() {
  if (delegate_)
    delegate_->HandlePointerActivate();
}
