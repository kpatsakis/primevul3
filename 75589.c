void ExtensionService::UpdateExtensionBlacklist(
  const std::vector<std::string>& blacklist) {
  std::set<std::string> blacklist_set;
  for (unsigned int i = 0; i < blacklist.size(); ++i) {
    if (Extension::IdIsValid(blacklist[i])) {
      blacklist_set.insert(blacklist[i]);
    }
  }
  extension_prefs_->UpdateBlacklist(blacklist_set);
  std::vector<std::string> to_be_removed;
  for (ExtensionList::const_iterator iter = extensions_.begin();
       iter != extensions_.end(); ++iter) {
    const Extension* extension = (*iter);
    if (blacklist_set.find(extension->id()) != blacklist_set.end()) {
      to_be_removed.push_back(extension->id());
    }
  }

  for (unsigned int i = 0; i < to_be_removed.size(); ++i) {
    UnloadExtension(to_be_removed[i], extension_misc::UNLOAD_REASON_DISABLE);
  }
}
