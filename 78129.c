void WebContentsImpl::OnDidFailLoadWithError(
    int64 frame_id,
    const GURL& url,
    bool is_main_frame,
    int error_code,
    const string16& error_description) {
  GURL validated_url(url);
  RenderProcessHost* render_process_host = message_source_->GetProcess();
  RenderViewHost::FilterURL(render_process_host, false, &validated_url);
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidFailLoad(frame_id, validated_url, is_main_frame,
                                error_code, error_description,
                                message_source_));
}
