PluginInstallerInfoBarDelegate* PluginObserver::GetPluginInstaller() {
  if (plugin_installer_ == NULL)
    plugin_installer_.reset(new PluginInstallerInfoBarDelegate(tab_contents()));
  return plugin_installer_->AsPluginInstallerInfoBarDelegate();
}
