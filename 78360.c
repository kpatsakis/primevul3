void RenderThreadImpl::OnPurgePluginListCache(bool reload_pages) {
  EnsureWebKitInitialized();
  webkit_platform_support_->set_plugin_refresh_allowed(false);
  blink::resetPluginCache(reload_pages);
  webkit_platform_support_->set_plugin_refresh_allowed(true);

  FOR_EACH_OBSERVER(RenderProcessObserver, observers_, PluginListChanged());
}
