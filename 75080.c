InterstitialPage::InterstitialPage(TabContents* tab,
                                   bool new_navigation,
                                   const GURL& url)
    : tab_(tab),
      url_(url),
      new_navigation_(new_navigation),
      should_discard_pending_nav_entry_(new_navigation),
      enabled_(true),
      action_taken_(NO_ACTION),
      render_view_host_(NULL),
      original_child_id_(tab->render_view_host()->process()->id()),
      original_rvh_id_(tab->render_view_host()->routing_id()),
      should_revert_tab_title_(false),
      resource_dispatcher_host_notified_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(rvh_view_delegate_(
          new InterstitialPageRVHViewDelegate(this))) {
  renderer_preferences_util::UpdateFromSystemSettings(
      &renderer_preferences_, tab_->profile());

  InitInterstitialPageMap();
  DCHECK(new_navigation || !tab->controller().pending_entry());
}
