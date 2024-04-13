bool IsWebStoreURL(Profile* profile, const GURL& url) {
  ExtensionService* service = profile->GetExtensionService();
  const Extension* store = service->GetWebStoreApp();
  if (!store) {
    NOTREACHED();
    return false;
  }
  return (service->GetExtensionByWebExtent(url) == store);
}
