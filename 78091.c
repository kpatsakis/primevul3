double WebContentsImpl::GetZoomLevel() const {
  HostZoomMapImpl* zoom_map = static_cast<HostZoomMapImpl*>(
      HostZoomMap::GetForBrowserContext(GetBrowserContext()));
  if (!zoom_map)
    return 0;

  double zoom_level;
  if (temporary_zoom_settings_) {
    zoom_level = zoom_map->GetTemporaryZoomLevel(
        GetRenderProcessHost()->GetID(), GetRenderViewHost()->GetRoutingID());
  } else {
    GURL url;
    NavigationEntry* active_entry = GetController().GetActiveEntry();
    url = active_entry ? active_entry->GetURL() : GURL::EmptyGURL();
    zoom_level = zoom_map->GetZoomLevelForHostAndScheme(url.scheme(),
        net::GetHostOrSpecFromURL(url));
  }
  return zoom_level;
}
