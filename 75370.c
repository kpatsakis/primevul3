OutdatedPluginInfoBarDelegate::~OutdatedPluginInfoBarDelegate() {
  UserMetrics::RecordAction(UserMetricsAction("OutdatedPluginInfobar.Closed"));
}
