void InterstitialPage::DidNavigate(
    RenderViewHost* render_view_host,
    const ViewHostMsg_FrameNavigate_Params& params) {
  if (!enabled_) {
    DontProceed();
    return;
  }

  render_view_host_->view()->Show();
  tab_->set_interstitial_page(this);

  RenderWidgetHostView* rwh_view = tab_->render_view_host()->view();

  if (rwh_view) {
    if (rwh_view->HasFocus())
      Focus();

    rwh_view->Hide();
  }

  tab_->SetIsLoading(false, NULL);
}
