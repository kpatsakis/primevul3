 BlockedPluginInfoBarDelegate::~BlockedPluginInfoBarDelegate() {
  UserMetrics::RecordAction(UserMetricsAction("BlockedPluginInfobar.Closed"));
}
