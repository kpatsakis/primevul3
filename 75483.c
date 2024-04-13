bool ExtensionService::CanCrossIncognito(const Extension* extension) {
  return IsIncognitoEnabled(extension->id()) &&
      !extension->incognito_split_mode();
}
