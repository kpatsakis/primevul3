void RenderFrameImpl::didFinishResourceLoad(blink::WebLocalFrame* frame,
                                            unsigned identifier) {
  DCHECK(!frame_ || frame_ == frame);
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDataSource(frame->dataSource());
  if (!internal_data->use_error_page())
    return;

  if (render_view_->devtools_agent_->IsAttached())
    return;

  std::string error_domain = "http";
  int http_status_code = internal_data->http_status_code();
  if (GetContentClient()->renderer()->HasErrorPage(
          http_status_code, &error_domain)) {
    WebURLError error;
    error.unreachableURL = frame->document().url();
    error.domain = WebString::fromUTF8(error_domain);
    error.reason = http_status_code;
    LoadNavigationErrorPage(frame->dataSource()->request(), error, true);
  }
}
