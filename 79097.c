void WebURLLoaderImpl::Context::SetDefersLoading(bool value) {
  if (bridge_)
    bridge_->SetDefersLoading(value);
}
