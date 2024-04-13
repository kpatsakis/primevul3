void RenderFrameImpl::didReceiveResponse(
    blink::WebLocalFrame* frame,
    unsigned identifier,
    const blink::WebURLResponse& response) {
  DCHECK(!frame_ || frame_ == frame);
  if (!frame->provisionalDataSource() || frame->parent())
    return;

  if (frame->isViewSourceModeEnabled())
    return;

  DocumentState* document_state =
      DocumentState::FromDataSource(frame->provisionalDataSource());
  int http_status_code = response.httpStatusCode();

  WebURLResponseExtraDataImpl* extra_data =
      GetExtraDataFromResponse(response);
  if (extra_data) {
    document_state->set_was_fetched_via_spdy(
        extra_data->was_fetched_via_spdy());
    document_state->set_was_npn_negotiated(
        extra_data->was_npn_negotiated());
    document_state->set_npn_negotiated_protocol(
        extra_data->npn_negotiated_protocol());
    document_state->set_was_alternate_protocol_available(
        extra_data->was_alternate_protocol_available());
    document_state->set_connection_info(
        extra_data->connection_info());
    document_state->set_was_fetched_via_proxy(
        extra_data->was_fetched_via_proxy());
  }
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);
  internal_data->set_http_status_code(http_status_code);
  internal_data->set_use_error_page(true);
}
