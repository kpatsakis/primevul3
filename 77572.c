bool BrowserNonClientFrameViewAura::ShouldTabIconViewAnimate() const {
  content::WebContents* current_tab = browser_view()->GetSelectedWebContents();
  return current_tab ? current_tab->IsLoading() : false;
}
