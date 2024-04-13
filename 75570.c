void ExtensionService::SetInstalledAppForRenderer(int renderer_child_id,
                                                  const Extension* app) {
  installed_app_hosts_[renderer_child_id] = app;
}
