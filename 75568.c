void ExtensionService::SetBeingUpgraded(const Extension* extension,
                                         bool value) {
  extension_runtime_data_[extension->id()].being_upgraded = value;
}
