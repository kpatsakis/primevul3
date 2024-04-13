void ExtensionService::SetAllowFileAccess(const Extension* extension,
                                          bool allow) {
  bool old_allow = AllowFileAccess(extension);
  if (allow == old_allow)
    return;

  extension_prefs_->SetAllowFileAccess(extension->id(), allow);

  bool extension_is_enabled = std::find(extensions_.begin(), extensions_.end(),
                                        extension) != extensions_.end();
  if (extension_is_enabled)
    ReloadExtension(extension->id());
}
