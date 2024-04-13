void PluginInfoMessageFilter::Context::GetPluginContentSetting(
    const WebPluginInfo& plugin,
    const GURL& policy_url,
    const GURL& plugin_url,
    const std::string& resource,
    ContentSetting* setting,
    bool* uses_default_content_setting) const {
  bool is_nacl_plugin = (plugin.name == ASCIIToUTF16(
      chrome::ChromeContentClient::kNaClPluginName));

  scoped_ptr<base::Value> value;
  content_settings::SettingInfo info;
  bool uses_plugin_specific_setting = false;
  if (is_nacl_plugin) {
    value.reset(
        host_content_settings_map_->GetWebsiteSetting(
            policy_url, policy_url, CONTENT_SETTINGS_TYPE_JAVASCRIPT,
            std::string(), &info));
  } else {
    value.reset(
        host_content_settings_map_->GetWebsiteSetting(
            policy_url, plugin_url, CONTENT_SETTINGS_TYPE_PLUGINS, resource,
            &info));
    if (value.get()) {
      uses_plugin_specific_setting = true;
    } else {
      value.reset(host_content_settings_map_->GetWebsiteSetting(
          policy_url, plugin_url, CONTENT_SETTINGS_TYPE_PLUGINS, std::string(),
          &info));
    }
  }
  *setting = content_settings::ValueToContentSetting(value.get());
  *uses_default_content_setting =
      !uses_plugin_specific_setting &&
      info.primary_pattern == ContentSettingsPattern::Wildcard() &&
      info.secondary_pattern == ContentSettingsPattern::Wildcard();
}
