void ExtensionService::SetIsIncognitoEnabled(
    const std::string& extension_id, bool enabled) {
  const Extension* extension = GetInstalledExtension(extension_id);
  if (extension && extension->location() == Extension::COMPONENT) {
    NOTREACHED();
    return;
  }

  bool old_enabled = extension_prefs_->IsIncognitoEnabled(extension_id);
  if (enabled == old_enabled)
    return;

  extension_prefs_->SetIsIncognitoEnabled(extension_id, enabled);

  const Extension* enabled_extension = GetExtensionById(extension_id, false);
  if (enabled_extension) {
    NotifyExtensionUnloaded(
        enabled_extension, extension_misc::UNLOAD_REASON_DISABLE);
    NotifyExtensionLoaded(enabled_extension);
  }

  if (extension)
    SyncExtensionChangeIfNeeded(*extension);
}
