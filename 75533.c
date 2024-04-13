bool ExtensionService::IsExtensionEnabled(
    const std::string& extension_id) const {
  const Extension* extension =
      GetExtensionByIdInternal(extension_id, true, false, true);
  if (extension)
    return true;

  extension =
      GetExtensionByIdInternal(extension_id, false, true, false);
  if (extension)
    return false;

  return !extension_prefs_->IsExtensionDisabled(extension_id) &&
      !extension_prefs_->IsExternalExtensionUninstalled(extension_id);
}
