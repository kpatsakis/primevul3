void WebContentsImpl::LostCapture() {
  if (delegate_)
    delegate_->LostCapture();
}
