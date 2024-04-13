void ProfileImplIOData::Handle::ClearNetworkingHistorySince(
    base::Time time) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  LazyInitialize();

  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::Bind(
          &ClearNetworkingHistorySinceOnIOThread,
          io_data_,
          time));
}
