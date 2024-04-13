void PPB_URLLoader_Impl::willSendRequest(
    WebURLLoader* loader,
    WebURLRequest& new_request,
    const WebURLResponse& redirect_response) {
  if (!request_data_.follow_redirects) {
    SaveResponse(redirect_response);
    SetDefersLoading(true);
    RunCallback(PP_OK);
  }
}
