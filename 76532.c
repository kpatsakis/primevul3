int32_t PPB_URLLoader_Impl::FollowRedirect(
    scoped_refptr<TrackedCallback> callback) {
  int32_t rv = ValidateCallback(callback);
  if (rv != PP_OK)
    return rv;

  SetDefersLoading(false);  // Allow the redirect to continue.
  RegisterCallback(callback);
  return PP_OK_COMPLETIONPENDING;
}
