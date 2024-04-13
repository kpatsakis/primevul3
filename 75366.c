OutdatedPluginInfoBarDelegate::OutdatedPluginInfoBarDelegate(
    TabContents* tab_contents,
    const string16& utf16_name,
    const GURL& update_url)
    : PluginInfoBarDelegate(tab_contents, utf16_name),
      update_url_(update_url) {
  UserMetrics::RecordAction(UserMetricsAction("OutdatedPluginInfobar.Shown"));
  std::string name = UTF16ToUTF8(utf16_name);
  if (name == webkit::npapi::PluginGroup::kJavaGroupName)
    UserMetrics::RecordAction(
        UserMetricsAction("OutdatedPluginInfobar.Shown.Java"));
  else if (name == webkit::npapi::PluginGroup::kQuickTimeGroupName)
    UserMetrics::RecordAction(
        UserMetricsAction("OutdatedPluginInfobar.Shown.QuickTime"));
  else if (name == webkit::npapi::PluginGroup::kShockwaveGroupName)
    UserMetrics::RecordAction(
        UserMetricsAction("OutdatedPluginInfobar.Shown.Shockwave"));
  else if (name == webkit::npapi::PluginGroup::kRealPlayerGroupName)
    UserMetrics::RecordAction(
        UserMetricsAction("OutdatedPluginInfobar.Shown.RealPlayer"));
  else if (name == webkit::npapi::PluginGroup::kSilverlightGroupName)
    UserMetrics::RecordAction(
        UserMetricsAction("OutdatedPluginInfobar.Shown.Silverlight"));
  else if (name == webkit::npapi::PluginGroup::kAdobeReaderGroupName)
    UserMetrics::RecordAction(
        UserMetricsAction("OutdatedPluginInfobar.Shown.Reader"));
}
