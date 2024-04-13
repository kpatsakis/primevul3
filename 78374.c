void RenderThreadImpl::RemoveObserver(RenderProcessObserver* observer) {
  observers_.RemoveObserver(observer);
}
