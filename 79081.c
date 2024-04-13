void WebURLLoaderImpl::Context::Cancel() {
  if (bridge_)
    bridge_->Cancel();

  if (multipart_delegate_)
    multipart_delegate_->Cancel();

  client_ = NULL;
  loader_ = NULL;
}
