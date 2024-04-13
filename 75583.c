void ExtensionService::UnregisterComponentExtension(
    const ComponentExtensionInfo& info) {
  RegisteredComponentExtensions new_component_extension_manifests;
  for (RegisteredComponentExtensions::iterator it =
           component_extension_manifests_.begin();
       it != component_extension_manifests_.end(); ++it) {
    if (!it->Equals(info))
      new_component_extension_manifests.push_back(*it);
  }
  component_extension_manifests_.swap(new_component_extension_manifests);
}
