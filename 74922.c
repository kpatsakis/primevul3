void RenderView::didReceiveResponse(
    WebFrame* frame, unsigned identifier, const WebURLResponse& response) {

  SiteIsolationMetrics::LogMimeTypeForCrossOriginRequest(frame,
                                                         identifier,
                                                         response);

  if (!frame->provisionalDataSource() || frame->parent())
    return;

  if (frame->isViewSourceModeEnabled())
    return;

  NavigationState* navigation_state =
      NavigationState::FromDataSource(frame->provisionalDataSource());
  CHECK(navigation_state);

  navigation_state->set_was_fetched_via_spdy(response.wasFetchedViaSPDY());
  navigation_state->set_was_npn_negotiated(response.wasNpnNegotiated());
  navigation_state->set_was_alternate_protocol_available(
      response.wasAlternateProtocolAvailable());
  navigation_state->set_was_fetched_via_proxy(response.wasFetchedViaProxy());

  if (response.httpStatusCode() != 404)
    return;

  if (!GetAlternateErrorPageURL(response.url(), HTTP_404).is_valid())
    return;

  navigation_state->set_postpone_loading_data(true);
  navigation_state->clear_postponed_data();
}
