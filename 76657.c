bool PluginInfoMessageFilter::Context::FindEnabledPlugin(
    int render_view_id,
    const GURL& url,
    const GURL& top_origin_url,
    const std::string& mime_type,
    ChromeViewHostMsg_GetPluginInfo_Status* status,
    WebPluginInfo* plugin,
    std::string* actual_mime_type) const {
  bool allow_wildcard = true;
  std::vector<WebPluginInfo> matching_plugins;
  std::vector<std::string> mime_types;
  PluginService::GetInstance()->GetPluginInfoArray(
      url, mime_type, allow_wildcard, &matching_plugins, &mime_types);
  content::PluginServiceFilter* filter =
      PluginService::GetInstance()->GetFilter();
  bool found = false;
  for (size_t i = 0; i < matching_plugins.size(); ++i) {
    bool enabled = !filter || filter->ShouldUsePlugin(render_process_id_,
                                                      render_view_id,
                                                      resource_context_,
                                                      url,
                                                      top_origin_url,
                                                      &matching_plugins[i]);
    if (!found || enabled) {
      *plugin = matching_plugins[i];
      *actual_mime_type = mime_types[i];
      if (enabled) {
        return true;
      }
      found = true;
    }
  }

  if (found)
    status->value = ChromeViewHostMsg_GetPluginInfo_Status::kDisabled;
  else
    status->value = ChromeViewHostMsg_GetPluginInfo_Status::kNotFound;
  return false;
}
