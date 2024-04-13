void WebURLLoaderImpl::loadSynchronously(const WebURLRequest& request,
                                         WebURLResponse& response,
                                         WebURLError& error,
                                         WebData& data) {
  ResourceLoaderBridge::SyncLoadResponse sync_load_response;
  context_->Start(request, &sync_load_response, platform_);

  const GURL& final_url = sync_load_response.url;

  int error_code = sync_load_response.error_code;
  if (error_code != net::OK) {
    response.setURL(final_url);
    error.domain = WebString::fromUTF8(net::kErrorDomain);
    error.reason = error_code;
    error.unreachableURL = final_url;
    return;
  }

  PopulateURLResponse(final_url, sync_load_response, &response);

  data.assign(sync_load_response.data.data(),
              sync_load_response.data.size());
}
