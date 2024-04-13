ProfileImplIOData::Handle::GetChromeURLDataManagerBackendGetter() const {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  LazyInitialize();
  return base::Bind(&ProfileIOData::GetChromeURLDataManagerBackend,
                    base::Unretained(io_data_));
}
