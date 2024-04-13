WebContents* WebContents::Create(const WebContents::CreateParams& params) {
  return WebContentsImpl::CreateWithOpener(
      params, static_cast<WebContentsImpl*>(params.opener));
}
