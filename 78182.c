void WebContentsImpl::SetUserAgentOverride(const std::string& override) {
  if (GetUserAgentOverride() == override)
    return;

  renderer_preferences_.user_agent_override = override;

  RenderViewHost* host = GetRenderViewHost();
  if (host)
    host->SyncRendererPrefs();

  NavigationEntry* entry = controller_.GetActiveEntry();
  if (is_loading_ && entry != NULL && entry->GetIsOverridingUserAgent())
    controller_.ReloadIgnoringCache(true);

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    UserAgentOverrideSet(override));
}
