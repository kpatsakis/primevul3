void WebURLLoaderImpl::Context::DidChangePriority(
    WebURLRequest::Priority new_priority) {
  if (bridge_)
    bridge_->DidChangePriority(
        ConvertWebKitPriorityToNetPriority(new_priority));
}
