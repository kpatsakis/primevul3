bool ExtensionService::IsIncognitoEnabled(
    const std::string& extension_id) const {
  const Extension* extension = GetInstalledExtension(extension_id);
  if (extension && extension->location() == Extension::COMPONENT)
    return true;

  return extension_prefs_->IsIncognitoEnabled(extension_id);
}
