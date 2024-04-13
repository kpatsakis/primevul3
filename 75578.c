void ExtensionService::TrackTerminatedExtension(const Extension* extension) {
  if (terminated_extension_ids_.insert(extension->id()).second)
    terminated_extensions_.push_back(make_scoped_refptr(extension));

  UnloadExtension(extension->id(), extension_misc::UNLOAD_REASON_DISABLE);
}
