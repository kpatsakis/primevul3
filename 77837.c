void RenderFrameImpl::LoadNavigationErrorPage(
    const WebURLRequest& failed_request,
    const WebURLError& error,
    bool replace) {
  std::string error_html;
  GetContentClient()->renderer()->GetNavigationErrorStrings(
      render_view(), frame_, failed_request, error, &error_html, NULL);

  frame_->loadHTMLString(error_html,
                         GURL(kUnreachableWebDataURL),
                         error.unreachableURL,
                         replace);
}
