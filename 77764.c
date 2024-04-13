void CurrentHistoryCleaner::DidCommitProvisionalLoadForFrame(
    int64 frame_id,
    const string16& frame_unique_name,
    bool is_main_frame,
    const GURL& url,
    content::PageTransition transition_type,
    content::RenderViewHost* render_view_host) {
  if (!is_main_frame)
    return;

  content::NavigationController* nc = &web_contents()->GetController();
  HistoryService* hs = HistoryServiceFactory::GetForProfile(
      Profile::FromBrowserContext(web_contents()->GetBrowserContext()),
      Profile::IMPLICIT_ACCESS);

  if (history_index_to_remove_ < nc->GetLastCommittedEntryIndex()) {
    content::NavigationEntry* entry =
        nc->GetEntryAtIndex(history_index_to_remove_);
    if (signin::IsContinueUrlForWebBasedSigninFlow(entry->GetURL())) {
      hs->DeleteURL(entry->GetURL());
      nc->RemoveEntryAtIndex(history_index_to_remove_);
      delete this;  // Success.
    }
  }
}
