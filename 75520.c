const Extension* ExtensionService::GetWebStoreApp() {
  return GetExtensionById(extension_misc::kWebStoreAppId, false);
}
