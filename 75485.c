void ExtensionService::CheckAdminBlacklist() {
  std::vector<std::string> to_be_removed;
  for (ExtensionList::const_iterator iter = extensions_.begin();
       iter != extensions_.end(); ++iter) {
    const Extension* extension = (*iter);
    if (!extension_prefs_->IsExtensionAllowedByPolicy(extension->id()))
      to_be_removed.push_back(extension->id());
  }

  for (unsigned int i = 0; i < to_be_removed.size(); ++i)
    UnloadExtension(to_be_removed[i], extension_misc::UNLOAD_REASON_DISABLE);
}
