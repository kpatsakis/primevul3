void RenderView::AltErrorPageFinished(WebFrame* frame,
                                      const WebURLError& original_error,
                                      const std::string& html) {

  const std::string* html_to_load = &html;
  if (html.empty()) {
    NavigationState* navigation_state =
        NavigationState::FromDataSource(frame->dataSource());
    html_to_load = &navigation_state->postponed_data();
  }
  LoadNavigationErrorPage(
      frame, WebURLRequest(), original_error, *html_to_load, true);
}
