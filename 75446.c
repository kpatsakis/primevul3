void ExtensionPrefs::UpdateManifest(const Extension* extension) {
  if (extension->location() != Extension::LOAD) {
    const DictionaryValue* extension_dict = GetExtensionPref(extension->id());
    if (!extension_dict)
      return;
    DictionaryValue* old_manifest = NULL;
    bool update_required =
        !extension_dict->GetDictionary(kPrefManifest, &old_manifest) ||
        !extension->manifest_value()->Equals(old_manifest);
    if (update_required) {
      UpdateExtensionPref(extension->id(), kPrefManifest,
                          extension->manifest_value()->DeepCopy());
    }
  }
}
