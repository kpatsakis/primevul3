void NavigationController::RendererDidNavigateToExistingPage(
    const ViewHostMsg_FrameNavigate_Params& params) {
  DCHECK(PageTransition::IsMainFrame(params.transition));

  int entry_index = GetEntryIndexWithPageID(tab_contents_->GetSiteInstance(),
                                            params.page_id);
  DCHECK(entry_index >= 0 &&
         entry_index < static_cast<int>(entries_.size()));
  NavigationEntry* entry = entries_[entry_index].get();

  entry->set_url(params.url);
  if (entry->update_virtual_url_with_url())
    UpdateVirtualURLToURL(entry, params.url);
  DCHECK(entry->site_instance() == NULL ||
         entry->site_instance() == tab_contents_->GetSiteInstance());
  entry->set_site_instance(tab_contents_->GetSiteInstance());

  entry->set_has_post_data(params.is_post);

  if (pending_entry_)
    DiscardNonCommittedEntriesInternal();

  last_committed_entry_index_ =
      GetEntryIndexWithPageID(tab_contents_->GetSiteInstance(), params.page_id);
}
