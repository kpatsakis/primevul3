bool BlockedPluginInfoBarDelegate::Accept() {
  UserMetrics::RecordAction(
      UserMetricsAction("BlockedPluginInfobar.AllowThisTime"));
  return PluginInfoBarDelegate::Cancel();
}
