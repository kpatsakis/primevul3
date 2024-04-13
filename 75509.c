const Extension* ExtensionService::GetInstalledApp(const GURL& url) {
  const Extension* app = GetExtensionByWebExtent(url);
  if (app)
    return app;

  app = GetExtensionByURL(url);
  if (app && app->is_app())
    return app;

  return NULL;
}
