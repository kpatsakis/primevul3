const Extension* ExtensionService::GetInstalledAppForRenderer(
    int renderer_child_id) {
  InstalledAppMap::iterator i = installed_app_hosts_.find(renderer_child_id);
  if (i == installed_app_hosts_.end())
    return NULL;
  return i->second;
}
