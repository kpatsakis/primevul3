void OutdatedPluginInfoBarDelegate::InfoBarDismissed() {
  UserMetrics::RecordAction(
      UserMetricsAction("OutdatedPluginInfobar.Dismissed"));
}
