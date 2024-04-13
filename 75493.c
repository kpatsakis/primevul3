void ExtensionService::EnableExtension(const std::string& extension_id) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (IsExtensionEnabled(extension_id))
    return;

  extension_prefs_->SetExtensionState(extension_id, Extension::ENABLED);

  const Extension* extension =
      GetExtensionByIdInternal(extension_id, false, true, false);
  if (!extension)
    return;

  extensions_.push_back(make_scoped_refptr(extension));
  ExtensionList::iterator iter = std::find(disabled_extensions_.begin(),
                                           disabled_extensions_.end(),
                                           extension);
  disabled_extensions_.erase(iter);

  extension_prefs_->SetBrowserActionVisibility(extension, true);

  NotifyExtensionLoaded(extension);

  SyncExtensionChangeIfNeeded(*extension);
}
