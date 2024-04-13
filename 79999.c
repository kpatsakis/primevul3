void RenderViewImpl::LoadNavigationErrorPage(
    WebFrame* frame,
    const WebURLRequest& failed_request,
    const WebURLError& error,
    const std::string& html,
    bool replace) {
  std::string alt_html;
  const std::string* error_html;

  if (!html.empty()) {
    error_html = &html;
  } else {
    GetContentClient()->renderer()->GetNavigationErrorStrings(
        failed_request, error, &alt_html, NULL);
    error_html = &alt_html;
  }

  frame->loadHTMLString(*error_html,
                        GURL(kUnreachableWebDataURL),
                        error.unreachableURL,
                        replace);
}
