void RenderView::LoadNavigationErrorPage(WebFrame* frame,
                                         const WebURLRequest& failed_request,
                                         const WebURLError& error,
                                         const std::string& html,
                                         bool replace) {
  GURL failed_url = error.unreachableURL;

  std::string alt_html;
  if (html.empty()) {
    int resource_id;
    DictionaryValue error_strings;
    if (error.reason == net::ERR_CACHE_MISS &&
        EqualsASCII(failed_request.httpMethod(), "POST")) {
      GetFormRepostErrorValues(failed_url, &error_strings);
      resource_id = IDR_ERROR_NO_DETAILS_HTML;
    } else {
      GetLocalizedErrorValues(error, &error_strings);
      resource_id = IDR_NET_ERROR_HTML;
    }

    alt_html = GetAltHTMLForTemplate(error_strings, resource_id);
  } else {
    alt_html = html;
  }

  frame->loadHTMLString(alt_html,
                        GURL(kUnreachableWebDataURL),
                        failed_url,
                        replace);
}
