void PluginObserver::OnCrashedPlugin(const FilePath& plugin_path) {
  DCHECK(!plugin_path.value().empty());

  string16 plugin_name = plugin_path.LossyDisplayName();
  webkit::npapi::WebPluginInfo plugin_info;
  if (webkit::npapi::PluginList::Singleton()->GetPluginInfoByPath(
          plugin_path, &plugin_info) &&
      !plugin_info.name.empty()) {
    plugin_name = plugin_info.name;
#if defined(OS_MACOSX)
    const std::string kPluginExtension = ".plugin";
    if (EndsWith(plugin_name, ASCIIToUTF16(kPluginExtension), true))
      plugin_name.erase(plugin_name.length() - kPluginExtension.length());
#endif  // OS_MACOSX
  }
  gfx::Image* icon = &ResourceBundle::GetSharedInstance().GetNativeImageNamed(
      IDR_INFOBAR_PLUGIN_CRASHED);
  tab_contents_->AddInfoBar(new SimpleAlertInfoBarDelegate(tab_contents(),
      icon,
      l10n_util::GetStringFUTF16(IDS_PLUGIN_CRASHED_PROMPT, plugin_name),
      true));
}
