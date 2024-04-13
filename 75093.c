void InterstitialPage::UpdateTitle(RenderViewHost* render_view_host,
                                   int32 page_id,
                                   const std::wstring& title) {
  DCHECK(render_view_host == render_view_host_);
  NavigationEntry* entry = tab_->controller().GetActiveEntry();
  if (!entry) {
    NOTREACHED();
    return;
  }

  if (!new_navigation_ && !should_revert_tab_title_) {
    original_tab_title_ = UTF16ToWideHack(entry->title());
    should_revert_tab_title_ = true;
  }
  entry->set_title(WideToUTF16Hack(title));
  tab_->NotifyNavigationStateChanged(TabContents::INVALIDATE_TITLE);
}
