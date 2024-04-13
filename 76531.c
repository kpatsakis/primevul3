int32_t PPB_URLLoader_Impl::FinishStreamingToFile(
    scoped_refptr<TrackedCallback> callback) {
  int32_t rv = ValidateCallback(callback);
  if (rv != PP_OK)
    return rv;
  if (!response_info_ || !response_info_->body())
    return PP_ERROR_FAILED;

  if (done_status_ != PP_OK_COMPLETIONPENDING)
    return done_status_;

  is_streaming_to_file_ = true;
  if (is_asynchronous_load_suspended_)
    SetDefersLoading(false);

  RegisterCallback(callback);
  return PP_OK_COMPLETIONPENDING;
}
