void InterstitialPage::Show() {
  if (tab_->interstitial_page()) {
    if (tab_->interstitial_page()->action_taken_ != NO_ACTION) {
      tab_->interstitial_page()->Hide();
    } else {
      if (new_navigation_ && tab_->interstitial_page()->new_navigation_)
        tab_->interstitial_page()->should_discard_pending_nav_entry_= false;
      tab_->interstitial_page()->DontProceed();
    }
  }

  TakeActionOnResourceDispatcher(BLOCK);
  notification_registrar_.Add(
      this, NotificationType::RENDER_WIDGET_HOST_DESTROYED,
      Source<RenderWidgetHost>(tab_->render_view_host()));

  InterstitialPageMap::const_iterator iter =
      tab_to_interstitial_page_->find(tab_);
  DCHECK(iter == tab_to_interstitial_page_->end());
  (*tab_to_interstitial_page_)[tab_] = this;

  if (new_navigation_) {
    NavigationEntry* entry = new NavigationEntry;
    entry->set_url(url_);
    entry->set_virtual_url(url_);
    entry->set_page_type(NavigationEntry::INTERSTITIAL_PAGE);

    UpdateEntry(entry);

    tab_->controller().AddTransientEntry(entry);
  }

  DCHECK(!render_view_host_);
  render_view_host_ = CreateRenderViewHost();
  CreateTabContentsView();

  std::string data_url = "data:text/html;charset=utf-8," +
                         EscapePath(GetHTMLContents());
  render_view_host_->NavigateToURL(GURL(data_url));

  notification_registrar_.Add(this, NotificationType::TAB_CONTENTS_DESTROYED,
                              Source<TabContents>(tab_));
  notification_registrar_.Add(this, NotificationType::NAV_ENTRY_COMMITTED,
      Source<NavigationController>(&tab_->controller()));
  notification_registrar_.Add(this, NotificationType::NAV_ENTRY_PENDING,
      Source<NavigationController>(&tab_->controller()));
}
