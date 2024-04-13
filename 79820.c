void SafeBrowsingBlockingPage::NotifySafeBrowsingUIManager(
    SafeBrowsingUIManager* ui_manager,
    const UnsafeResourceList& unsafe_resources,
    bool proceed) {
  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::Bind(&SafeBrowsingUIManager::OnBlockingPageDone,
                 ui_manager, unsafe_resources, proceed));
}
