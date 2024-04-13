int32_t PPB_URLLoader_Impl::Open(PP_Resource request_id,
                                 scoped_refptr<TrackedCallback> callback) {
  EnterResourceNoLock<PPB_URLRequestInfo_API> enter_request(request_id, true);
  if (enter_request.failed()) {
    Log(PP_LOGLEVEL_ERROR,
        "PPB_URLLoader.Open: invalid request resource ID. (Hint to C++ wrapper"
        " users: use the ResourceRequest constructor that takes an instance or"
        " else the request will be null.)");
    return PP_ERROR_BADARGUMENT;
  }
  return Open(enter_request.object()->GetData(), 0, callback);
}
