void WebContentsImpl::WorkerCrashed() {
  if (delegate_)
    delegate_->WorkerCrashed(this);
}
