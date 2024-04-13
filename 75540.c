const Extension* ExtensionService::LoadComponentExtension(
    const ComponentExtensionInfo &info) {
  JSONStringValueSerializer serializer(info.manifest);
  scoped_ptr<Value> manifest(serializer.Deserialize(NULL, NULL));
  if (!manifest.get()) {
    LOG(ERROR) << "Failed to parse manifest for extension";
    return NULL;
  }

  int flags = Extension::REQUIRE_KEY;
  if (Extension::ShouldDoStrictErrorChecking(Extension::COMPONENT))
    flags |= Extension::STRICT_ERROR_CHECKS;
  std::string error;
  scoped_refptr<const Extension> extension(Extension::Create(
      info.root_directory,
      Extension::COMPONENT,
      *static_cast<DictionaryValue*>(manifest.get()),
      flags,
      &error));
  if (!extension.get()) {
    LOG(ERROR) << error;
    return NULL;
  }
  AddExtension(extension);
  return extension;
}
