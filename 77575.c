ProfileImplIOData::AcquireIsolatedAppRequestContext(
    scoped_refptr<ChromeURLRequestContext> main_context,
    const std::string& app_id) const {
  scoped_refptr<ChromeURLRequestContext> app_request_context =
      InitializeAppRequestContext(main_context, app_id);
  DCHECK(app_request_context);
  return app_request_context;
}
