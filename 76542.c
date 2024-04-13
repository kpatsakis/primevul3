int32_t PPB_URLLoader_Impl::ValidateCallback(
    scoped_refptr<TrackedCallback> callback) {
  DCHECK(callback);

  if (TrackedCallback::IsPending(pending_callback_))
    return PP_ERROR_INPROGRESS;

  return PP_OK;
}
