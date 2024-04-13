void WebContentsImpl::LostMouseLock() {
  if (delegate_)
    delegate_->LostMouseLock();
}
