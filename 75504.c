const Extension* ExtensionService::GetExtensionById(
    const std::string& id, bool include_disabled) const {
  return GetExtensionByIdInternal(id, true, include_disabled, false);
}
