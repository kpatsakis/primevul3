WebContentsImpl* WebContentsImpl::CreateWithOpener(
    const WebContents::CreateParams& params,
    WebContentsImpl* opener) {
  TRACE_EVENT0("browser", "WebContentsImpl::CreateWithOpener");
  WebContentsImpl* new_contents = new WebContentsImpl(
      params.browser_context, opener);

  new_contents->Init(params);
  return new_contents;
}
