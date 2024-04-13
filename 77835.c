static bool IsNonLocalTopLevelNavigation(const GURL& url,
                                         WebFrame* frame,
                                         WebNavigationType type,
                                         bool is_form_post) {
  if (!IsTopLevelNavigation(frame))
    return false;

  if (!url.SchemeIs(kHttpScheme) && !url.SchemeIs(kHttpsScheme))
    return false;

  if (type != blink::WebNavigationTypeReload &&
      type != blink::WebNavigationTypeBackForward && !is_form_post) {
    blink::WebFrame* opener = frame->opener();
    if (!opener)
      return true;

    if (url.GetOrigin() != GURL(opener->document().url()).GetOrigin())
      return true;
  }
  return false;
}
