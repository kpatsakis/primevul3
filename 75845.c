NavigationType::Type NavigationController::ClassifyNavigation(
    const ViewHostMsg_FrameNavigate_Params& params) const {
  if (params.page_id == -1) {
    return NavigationType::NAV_IGNORE;
  }

  if (params.page_id > tab_contents_->GetMaxPageID()) {
    if (PageTransition::IsMainFrame(params.transition))
      return NavigationType::NEW_PAGE;

    if (!GetLastCommittedEntry())
      return NavigationType::NAV_IGNORE;

    return NavigationType::NEW_SUBFRAME;
  }

  int existing_entry_index = GetEntryIndexWithPageID(
      tab_contents_->GetSiteInstance(),
      params.page_id);
  if (existing_entry_index == -1) {
    NOTREACHED();
    return NavigationType::NAV_IGNORE;
  }
  NavigationEntry* existing_entry = entries_[existing_entry_index].get();

  if (!PageTransition::IsMainFrame(params.transition)) {
    DCHECK(GetLastCommittedEntry());
    return NavigationType::AUTO_SUBFRAME;
  }

  if (pending_entry_ &&
      existing_entry != pending_entry_ &&
      pending_entry_->page_id() == -1 &&
      existing_entry == GetLastCommittedEntry()) {
    return NavigationType::SAME_PAGE;
  }

  if (AreURLsInPageNavigation(existing_entry->url(), params.url))
    return NavigationType::IN_PAGE;

  return NavigationType::EXISTING_PAGE;
}
