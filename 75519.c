const Extension* ExtensionService::GetTerminatedExtension(
    const std::string& id) const {
  return GetExtensionByIdInternal(id, false, false, true);
}
