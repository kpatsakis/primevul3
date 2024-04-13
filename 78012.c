void WebContentsImpl::DidChangeVisibleSSLState() {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidChangeVisibleSSLState());
}
