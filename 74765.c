bool RenderView::IsNonLocalTopLevelNavigation(
    const GURL& url, WebKit::WebFrame* frame, WebKit::WebNavigationType type) {
  if (frame->parent() != NULL)
    return false;

  if (!url.SchemeIs("http") && !url.SchemeIs("https"))
    return false;

  GURL frame_url(frame->url());
  if (!frame_url.SchemeIs("http") && !frame_url.SchemeIs("https"))
    return false;

  GURL frame_origin = frame_url.GetOrigin();
  if (url.GetOrigin() != frame_origin || url.ref().empty()) {
    if (type == WebKit::WebNavigationTypeLinkClicked)
      return true;

    if (last_top_level_navigation_page_id_ != page_id_ &&
        type != WebKit::WebNavigationTypeReload &&
        type != WebKit::WebNavigationTypeFormSubmitted &&
        type != WebKit::WebNavigationTypeBackForward) {
      return true;
    }
  }
  if (type != WebKit::WebNavigationTypeReload &&
      type != WebKit::WebNavigationTypeFormSubmitted &&
      type != WebKit::WebNavigationTypeBackForward) {
    WebKit::WebFrame* opener = frame->opener();
    if (opener) {
      if (url.GetOrigin() != GURL(opener->url()).GetOrigin())
        return true;
    }
  }
  return false;
}
