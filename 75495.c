bool ExtensionService::ExtensionBindingsAllowed(const GURL& url) {
  const Extension* extension = GetExtensionByURL(url);
  if (extension && extension->GetType() != Extension::TYPE_HOSTED_APP)
    return true;

  if (!extension)
    extension = GetExtensionByWebExtent(url);
  return (extension && extension->location() == Extension::COMPONENT);
}
