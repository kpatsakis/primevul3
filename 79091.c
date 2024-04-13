void WebURLLoaderImpl::Context::OnReceivedCachedMetadata(
    const char* data, int len) {
  if (client_)
    client_->didReceiveCachedMetadata(loader_, data, len);
}
