ProfileImplIOData::Handle::GetResourceContextNoInit() const {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  return io_data_->GetResourceContext();
}
