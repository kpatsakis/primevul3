FilePath SavePackage::EnsureHtmlExtension(const FilePath& name) {
  FilePath::StringType ext = name.Extension();
  if (!ext.empty())
    ext.erase(ext.begin());  // Erase preceding '.'.
  std::string mime_type;
  if (!net::GetMimeTypeFromExtension(ext, &mime_type) ||
      !CanSaveAsComplete(mime_type)) {
    return FilePath(name.value() + FILE_PATH_LITERAL(".") +
                    kDefaultHtmlExtension);
  }
  return name;
}
