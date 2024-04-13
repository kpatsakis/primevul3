int32_t PPB_URLLoader_Impl::ReadResponseBody(
    void* buffer,
    int32_t bytes_to_read,
    scoped_refptr<TrackedCallback> callback) {
  int32_t rv = ValidateCallback(callback);
  if (rv != PP_OK)
    return rv;
  if (!response_info_ || response_info_->body())
    return PP_ERROR_FAILED;
  if (bytes_to_read <= 0 || !buffer)
    return PP_ERROR_BADARGUMENT;

  user_buffer_ = static_cast<char*>(buffer);
  user_buffer_size_ = bytes_to_read;

  if (!buffer_.empty())
    return FillUserBuffer();

  if (done_status_ != PP_OK_COMPLETIONPENDING) {
    user_buffer_ = NULL;
    user_buffer_size_ = 0;
    return done_status_;
  }

  RegisterCallback(callback);
  return PP_OK_COMPLETIONPENDING;
}
