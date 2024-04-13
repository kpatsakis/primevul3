RenderViewHost* InterstitialPage::CreateRenderViewHost() {
  RenderViewHost* render_view_host = new RenderViewHost(
      SiteInstance::CreateSiteInstance(tab()->profile()),
      this, MSG_ROUTING_NONE, kInvalidSessionStorageNamespaceId);
  return render_view_host;
}
