void WebContentsImpl::DidChangeLoadProgress(double progress) {
  if (delegate_)
    delegate_->LoadProgressChanged(this, progress);
}
