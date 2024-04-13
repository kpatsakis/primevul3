void WebContentsImpl::ViewFrameSource(const GURL& url,
                                      const PageState& page_state) {
  if (!delegate_)
    return;

  delegate_->ViewSourceForFrame(this, url, page_state);
}
