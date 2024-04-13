ProfileImplIOData::Handle::GetIsolatedAppRequestContextGetter(
    const std::string& app_id) const {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!app_id.empty());
  LazyInitialize();

  ChromeURLRequestContextGetterMap::iterator iter =
      app_request_context_getter_map_.find(app_id);
  if (iter != app_request_context_getter_map_.end())
    return iter->second;


  ChromeURLRequestContextGetter* context =
      ChromeURLRequestContextGetter::CreateOriginalForIsolatedApp(
          profile_, io_data_, app_id);
  app_request_context_getter_map_[app_id] = context;

  return context;
}
