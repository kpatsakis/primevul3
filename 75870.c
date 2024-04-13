void NavigationController::RendererDidNavigateToSamePage(
    const ViewHostMsg_FrameNavigate_Params& params) {
  NavigationEntry* existing_entry = GetEntryWithPageID(
      tab_contents_->GetSiteInstance(),
      params.page_id);

  existing_entry->set_unique_id(pending_entry_->unique_id());

  if (existing_entry->update_virtual_url_with_url())
    UpdateVirtualURLToURL(existing_entry, params.url);
  existing_entry->set_url(params.url);

  DiscardNonCommittedEntries();
}
