bool SandboxedExtensionUnpacker::RewriteCatalogFiles() {
  DictionaryValue catalogs;
  if (!ExtensionUnpacker::ReadMessageCatalogsFromFile(temp_dir_.path(),
                                                      &catalogs)) {
    ReportFailure("Could not read catalog data from disk.");
    return false;
  }

  for (DictionaryValue::key_iterator key_it = catalogs.begin_keys();
       key_it != catalogs.end_keys(); ++key_it) {
    DictionaryValue* catalog;
    if (!catalogs.GetDictionaryWithoutPathExpansion(*key_it, &catalog)) {
      ReportFailure("Invalid catalog data.");
      return false;
    }

    FilePath relative_path = FilePath::FromWStringHack(UTF8ToWide(*key_it));
    relative_path = relative_path.Append(Extension::kMessagesFilename);
    if (relative_path.IsAbsolute() || relative_path.ReferencesParent()) {
      ReportFailure("Invalid path for catalog.");
      return false;
    }
    FilePath path = extension_root_.Append(relative_path);

    std::string catalog_json;
    JSONStringValueSerializer serializer(&catalog_json);
    serializer.set_pretty_print(true);
    if (!serializer.Serialize(*catalog)) {
      ReportFailure("Error serializing catalog.");
      return false;
    }

    if (!file_util::WriteFile(path,
                              catalog_json.c_str(),
                              catalog_json.size())) {
      ReportFailure("Error saving catalog.");
      return false;
    }
  }

  return true;
}
