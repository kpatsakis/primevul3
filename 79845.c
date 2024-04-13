FilePath SavePackage::EnsureMimeExtension(const FilePath& name,
    const std::string& contents_mime_type) {
  FilePath::StringType ext = name.Extension().length() ?
      name.Extension().substr(1) : name.Extension();
  FilePath::StringType suggested_extension =
      ExtensionForMimeType(contents_mime_type);
  std::string mime_type;
  if (!suggested_extension.empty() &&
      !net::GetMimeTypeFromExtension(ext, &mime_type)) {
    return FilePath(name.value() + FILE_PATH_LITERAL(".") +
                    suggested_extension);
  }
  return name;
}
