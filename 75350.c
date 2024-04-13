bool BlockedPluginInfoBarDelegate::Cancel() {
  UserMetrics::RecordAction(
      UserMetricsAction("BlockedPluginInfobar.AlwaysAllow"));
  tab_contents_->profile()->GetHostContentSettingsMap()->AddExceptionForURL(
      tab_contents_->GetURL(), CONTENT_SETTINGS_TYPE_PLUGINS, std::string(),
      CONTENT_SETTING_ALLOW);
  return PluginInfoBarDelegate::Cancel();
}
