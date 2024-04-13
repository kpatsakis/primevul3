void WebContentsImpl::CopyMaxPageIDsFrom(WebContentsImpl* web_contents) {
  max_page_ids_ = web_contents->max_page_ids_;
}
