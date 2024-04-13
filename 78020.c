void WebContentsImpl::DidNavigateMainFramePostCommit(
    const LoadCommittedDetails& details,
    const ViewHostMsg_FrameNavigate_Params& params) {
  if (details.is_navigation_to_different_page()) {
    UpdateTargetURL(details.entry->GetPageID(), GURL());
  }

  if (!details.is_in_page) {
    displayed_insecure_content_ = false;
    SSLManager::NotifySSLInternalStateChanged(
        GetController().GetBrowserContext());
  }

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidNavigateMainFrame(details, params));
}
