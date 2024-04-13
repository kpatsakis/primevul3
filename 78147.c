void WebContentsImpl::OnUpdateFaviconURL(
    int32 page_id,
    const std::vector<FaviconURL>& candidates) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidUpdateFaviconURL(page_id, candidates));
}
