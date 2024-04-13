void ExtensionService::UnloadExtension(
    const std::string& extension_id,
    extension_misc::UnloadedExtensionReason reason) {
  scoped_refptr<const Extension> extension(
      GetExtensionByIdInternal(extension_id, true, true, false));

  if (!extension) {
    profile_->UnregisterExtensionWithRequestContexts(extension_id, reason);
    return;
  }

  unloaded_extension_paths_[extension->id()] = extension->path();

  disabled_extension_paths_.erase(extension->id());

  extension_runtime_data_.erase(extension_id);

  ExtensionList::iterator iter = std::find(disabled_extensions_.begin(),
                                           disabled_extensions_.end(),
                                           extension.get());
  if (iter != disabled_extensions_.end()) {
    UnloadedExtensionInfo details(extension, reason);
    details.already_disabled = true;
    disabled_extensions_.erase(iter);
    NotificationService::current()->Notify(
        chrome::NOTIFICATION_EXTENSION_UNLOADED,
        Source<Profile>(profile_),
        Details<UnloadedExtensionInfo>(&details));
    profile_->UnregisterExtensionWithRequestContexts(extension_id, reason);
    return;
  }

  iter = std::find(extensions_.begin(), extensions_.end(), extension.get());

  extensions_.erase(iter);

  NotifyExtensionUnloaded(extension.get(), reason);
}
