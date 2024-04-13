bool WebContentsImpl::GotResponseToLockMouseRequest(bool allowed) {
  return GetRenderViewHost() ?
      GetRenderViewHostImpl()->GotResponseToLockMouseRequest(allowed) : false;
}
