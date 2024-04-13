    ProfileImplIOData::Handle::GetResourceContext() const {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  LazyInitialize();
  return GetResourceContextNoInit();
}
