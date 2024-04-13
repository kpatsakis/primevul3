void SavePackage::InternalInit() {
  ResourceDispatcherHostImpl* rdh = ResourceDispatcherHostImpl::Get();
  if (!rdh) {
    NOTREACHED();
    return;
  }

  file_manager_ = rdh->save_file_manager();
  DCHECK(file_manager_);

  download_manager_ = static_cast<DownloadManagerImpl*>(
      BrowserContext::GetDownloadManager(
          web_contents()->GetBrowserContext()));
  DCHECK(download_manager_);

  RecordSavePackageEvent(SAVE_PACKAGE_STARTED);
}
