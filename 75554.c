void ExtensionService::OnExternalExtensionUpdateUrlFound(
    const std::string& id,
    const GURL& update_url,
    Extension::Location location) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  CHECK(Extension::IdIsValid(id));

  const Extension* extension = GetExtensionById(id, true);
  if (extension) {
    Extension::Location current = extension->location();
    if (current == Extension::GetHigherPriorityLocation(current, location))
      return;
  }
  pending_extension_manager()->AddFromExternalUpdateUrl(
      id, update_url, location);
   external_extension_url_added_ |= true;
 }
