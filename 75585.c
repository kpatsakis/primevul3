void ExtensionService::UntrackTerminatedExtension(const std::string& id) {
  std::string lowercase_id = StringToLowerASCII(id);
  if (terminated_extension_ids_.erase(lowercase_id) <= 0)
    return;

  for (ExtensionList::iterator iter = terminated_extensions_.begin();
       iter != terminated_extensions_.end(); ++iter) {
    if ((*iter)->id() == lowercase_id) {
      terminated_extensions_.erase(iter);
      return;
    }
  }
}
