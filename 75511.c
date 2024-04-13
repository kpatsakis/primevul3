const Extension* ExtensionService::GetInstalledExtension(
    const std::string& id) const {
  return GetExtensionByIdInternal(id, true, true, true);
}
