void ExtensionService::ReloadExtensions() {
  UnloadAllExtensions();
  LoadAllExtensions();
}
