void RenderFrameImpl::OnStop() {
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, OnStop());
}
