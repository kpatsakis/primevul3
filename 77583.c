ProfileImplIOData::Handle::GetMediaRequestContextGetter() const {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  LazyInitialize();
  if (!media_request_context_getter_) {
    media_request_context_getter_ =
        ChromeURLRequestContextGetter::CreateOriginalForMedia(
            profile_, io_data_);
  }
  return media_request_context_getter_;
}
