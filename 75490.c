void ExtensionService::ClearExtensionData(const GURL& extension_url) {
  scoped_refptr<ExtensionDataDeleter> deleter(
      new ExtensionDataDeleter(profile_, extension_url));
  deleter->StartDeleting();
}
