void RenderView::OnInstallMissingPlugin() {
  if (first_default_plugin_)
    first_default_plugin_->InstallMissingPlugin();
}
