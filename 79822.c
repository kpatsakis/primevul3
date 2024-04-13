void SafeBrowsingBlockingPage::OnProceed() {
  proceeded_ = true;
  RecordUserAction(PROCEED);
  FinishMalwareDetails(malware_details_proceed_delay_ms_);

  NotifySafeBrowsingUIManager(ui_manager_, unsafe_resources_, true);

  UnsafeResourceMap* unsafe_resource_map = GetUnsafeResourcesMap();
  UnsafeResourceMap::iterator iter = unsafe_resource_map->find(web_contents_);
  SafeBrowsingBlockingPage* blocking_page = NULL;
  if (iter != unsafe_resource_map->end() && !iter->second.empty()) {
    blocking_page = factory_->CreateSafeBrowsingPage(ui_manager_, web_contents_,
                                                     iter->second);
    unsafe_resource_map->erase(iter);
  }

  if (blocking_page)
    blocking_page->interstitial_page_->Show();
}
