void WebURLLoaderImpl::didChangePriority(WebURLRequest::Priority new_priority) {
  context_->DidChangePriority(new_priority);
}
