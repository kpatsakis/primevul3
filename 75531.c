bool ExtensionService::IsBeingUpgraded(const Extension* extension) {
  return extension_runtime_data_[extension->id()].being_upgraded;
}
