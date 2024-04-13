int WebContentsImpl::GetEmbeddedInstanceID() const {
  BrowserPluginGuest* guest = GetBrowserPluginGuest();
  if (guest)
    return guest->instance_id();
  return 0;
}
