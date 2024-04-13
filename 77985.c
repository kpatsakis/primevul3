void WebContentsImpl::AddObserver(WebContentsObserver* observer) {
  observers_.AddObserver(observer);
}
