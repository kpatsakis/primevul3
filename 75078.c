void InterstitialPage::Hide() {
  RenderWidgetHostView* old_view = tab_->render_view_host()->view();
  if (old_view) {
    old_view->Show();
  }

  if (render_view_host_->view() && render_view_host_->view()->HasFocus() &&
      tab_->render_view_host()->view()) {
    tab_->render_view_host()->view()->Focus();
  }

  render_view_host_->Shutdown();
  render_view_host_ = NULL;
  if (tab_->interstitial_page())
    tab_->remove_interstitial_page();
  NavigationEntry* entry = tab_->controller().GetActiveEntry();
  if (!new_navigation_ && should_revert_tab_title_) {
    entry->set_title(WideToUTF16Hack(original_tab_title_));
    tab_->NotifyNavigationStateChanged(TabContents::INVALIDATE_TITLE);
  }
  delete this;
}
