bool ExtensionService::CanLoadInIncognito(const Extension* extension) const {
  if (extension->is_hosted_app())
    return true;
  return extension->incognito_split_mode() &&
         IsIncognitoEnabled(extension->id());
}
