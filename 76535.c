int32_t PPB_URLLoader_Impl::Open(
    const ::ppapi::URLRequestInfoData& request_data,
    int requestor_pid,
    scoped_refptr<TrackedCallback> callback) {
  if (main_document_loader_)
    return PP_ERROR_INPROGRESS;

  int32_t rv = ValidateCallback(callback);
  if (rv != PP_OK)
    return rv;

  ::ppapi::URLRequestInfoData filled_in_request_data = request_data;

  if (URLRequestRequiresUniversalAccess(filled_in_request_data) &&
      !has_universal_access_) {
    Log(PP_LOGLEVEL_ERROR, "PPB_URLLoader.Open: The URL you're requesting is "
        " on a different security origin than your plugin. To request "
        " cross-origin resources, see "
        " PP_URLREQUESTPROPERTY_ALLOWCROSSORIGINREQUESTS.");
    return PP_ERROR_NOACCESS;
  }

  if (loader_.get())
    return PP_ERROR_INPROGRESS;

  WebFrame* frame = GetFrameForResource(this);
  if (!frame)
    return PP_ERROR_FAILED;
  WebURLRequest web_request;
  if (!CreateWebURLRequest(&filled_in_request_data, frame, &web_request))
    return PP_ERROR_FAILED;
  web_request.setRequestorProcessID(requestor_pid);

  request_data_ = filled_in_request_data;

  WebURLLoaderOptions options;
  if (has_universal_access_) {
    options.allowCredentials = true;
    options.crossOriginRequestPolicy =
        WebURLLoaderOptions::CrossOriginRequestPolicyAllow;
  } else {
    options.untrustedHTTP = true;
    if (request_data_.allow_cross_origin_requests) {
      options.allowCredentials = request_data_.allow_credentials;
      options.crossOriginRequestPolicy =
          WebURLLoaderOptions::CrossOriginRequestPolicyUseAccessControl;
    } else {
      options.allowCredentials = true;
    }
  }

  is_asynchronous_load_suspended_ = false;
  loader_.reset(frame->createAssociatedURLLoader(options));
  if (!loader_.get())
    return PP_ERROR_FAILED;

  loader_->loadAsynchronously(web_request, this);

  RegisterCallback(callback);
  return PP_OK_COMPLETIONPENDING;
}
