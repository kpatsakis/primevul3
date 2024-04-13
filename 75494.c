bool ExtensionService::ComponentExtensionInfo::Equals(
    const ComponentExtensionInfo& other) const {
  return other.manifest == manifest && other.root_directory == root_directory;
}
