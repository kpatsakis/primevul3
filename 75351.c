bool OutdatedPluginInfoBarDelegate::Cancel() {
  UserMetrics::RecordAction(
      UserMetricsAction("OutdatedPluginInfobar.AllowThisTime"));
  return PluginInfoBarDelegate::Cancel();
}
