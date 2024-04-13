void RenderThreadImpl::AddObserver(RenderProcessObserver* observer) {
  observers_.AddObserver(observer);
}
