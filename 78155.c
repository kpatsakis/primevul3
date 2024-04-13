void WebContentsImpl::RemoveObserver(WebContentsObserver* observer) {
  observers_.RemoveObserver(observer);
}
