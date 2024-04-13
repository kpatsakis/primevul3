void ExtensionService::UnloadComponentExtension(
    const ComponentExtensionInfo& info) {
  JSONStringValueSerializer serializer(info.manifest);
  scoped_ptr<Value> manifest(serializer.Deserialize(NULL, NULL));
  if (!manifest.get()) {
    LOG(ERROR) << "Failed to parse manifest for extension";
    return;
  }
  std::string public_key;
  std::string public_key_bytes;
  std::string id;
  if (!static_cast<DictionaryValue*>(manifest.get())->
      GetString(extension_manifest_keys::kPublicKey, &public_key) ||
      !Extension::ParsePEMKeyBytes(public_key, &public_key_bytes) ||
      !Extension::GenerateId(public_key_bytes, &id)) {
    LOG(ERROR) << "Failed to get extension id";
    return;
  }
  UnloadExtension(id, extension_misc::UNLOAD_REASON_DISABLE);
}
