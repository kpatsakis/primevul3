static bool CrossesExtensionExtents(WebFrame* frame, const GURL& new_url) {
  if (!RenderThread::current())
    return false;

  GURL old_url(frame->url());
  if (old_url.is_empty() && frame->opener())
    old_url = frame->opener()->url();

  std::string old_extension =
      RenderThread::current()->GetExtensionIdByURL(old_url);
  if (!old_extension.empty()) {
    if (RenderThread::current()->GetExtensionIdByBrowseExtent(new_url) ==
        old_extension) {
      return false;
    }
  }

  std::string new_extension =
      RenderThread::current()->GetExtensionIdByURL(new_url);
  return old_extension != new_extension;
}
