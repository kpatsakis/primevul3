void SafeBrowsingBlockingPage::ShowBlockingPage(
    SafeBrowsingUIManager* ui_manager,
    const UnsafeResource& unsafe_resource) {
  DVLOG(1) << __FUNCTION__ << " " << unsafe_resource.url.spec();
  WebContents* web_contents = tab_util::GetWebContentsByID(
      unsafe_resource.render_process_host_id, unsafe_resource.render_view_id);

  InterstitialPage* interstitial =
      InterstitialPage::GetInterstitialPage(web_contents);
  if (interstitial && !unsafe_resource.is_subresource) {
    interstitial->DontProceed();
    interstitial = NULL;
  }

  if (!interstitial) {
    std::vector<UnsafeResource> resources;
    resources.push_back(unsafe_resource);
    if (!factory_)
      factory_ = g_safe_browsing_blocking_page_factory_impl.Pointer();
    SafeBrowsingBlockingPage* blocking_page =
        factory_->CreateSafeBrowsingPage(ui_manager, web_contents, resources);
    blocking_page->interstitial_page_->Show();
    return;
  }

  UnsafeResourceMap* unsafe_resource_map = GetUnsafeResourcesMap();
  (*unsafe_resource_map)[web_contents].push_back(unsafe_resource);
}
