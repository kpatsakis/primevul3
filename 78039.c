    WebContentsImpl::GetBrowserPluginGuestManager() const {
  return static_cast<BrowserPluginGuestManager*>(
      GetBrowserContext()->GetUserData(
          browser_plugin::kBrowserPluginGuestManagerKeyName));
}
