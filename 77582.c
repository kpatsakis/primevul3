ProfileImplIOData::Handle::GetMainRequestContextGetter() const {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  LazyInitialize();
  if (!main_request_context_getter_) {
    main_request_context_getter_ =
        ChromeURLRequestContextGetter::CreateOriginal(
            profile_, io_data_);
  }
  return main_request_context_getter_;
}
