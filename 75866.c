void NavigationController::RendererDidNavigateInPage(
    const ViewHostMsg_FrameNavigate_Params& params, bool* did_replace_entry) {
  DCHECK(PageTransition::IsMainFrame(params.transition)) <<
      "WebKit should only tell us about in-page navs for the main frame.";
  NavigationEntry* existing_entry = GetEntryWithPageID(
      tab_contents_->GetSiteInstance(),
      params.page_id);

  NavigationEntry* new_entry = new NavigationEntry(*existing_entry);
  new_entry->set_page_id(params.page_id);
  if (new_entry->update_virtual_url_with_url())
    UpdateVirtualURLToURL(new_entry, params.url);
  new_entry->set_url(params.url);

  *did_replace_entry = true;
  InsertOrReplaceEntry(new_entry, true);
}
