bool DelegatedFrameHost::CanCopyToBitmap() const {
  return client_->GetCompositor() &&
         client_->GetLayer()->has_external_content();
}
