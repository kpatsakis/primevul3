void WebContentsImpl::UpdateTargetURL(int32 page_id, const GURL& url) {
  if (delegate_)
    delegate_->UpdateTargetURL(this, page_id, url);
}
