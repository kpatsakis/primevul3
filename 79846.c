const FilePath::CharType* SavePackage::ExtensionForMimeType(
    const std::string& contents_mime_type) {
  static const struct {
    const FilePath::CharType *mime_type;
    const FilePath::CharType *suggested_extension;
  } extensions[] = {
    { FILE_PATH_LITERAL("text/html"), kDefaultHtmlExtension },
    { FILE_PATH_LITERAL("text/xml"), FILE_PATH_LITERAL("xml") },
    { FILE_PATH_LITERAL("application/xhtml+xml"), FILE_PATH_LITERAL("xhtml") },
    { FILE_PATH_LITERAL("text/plain"), FILE_PATH_LITERAL("txt") },
    { FILE_PATH_LITERAL("text/css"), FILE_PATH_LITERAL("css") },
  };
#if defined(OS_POSIX)
  FilePath::StringType mime_type(contents_mime_type);
#elif defined(OS_WIN)
  FilePath::StringType mime_type(UTF8ToWide(contents_mime_type));
#endif  // OS_WIN
  for (uint32 i = 0; i < ARRAYSIZE_UNSAFE(extensions); ++i) {
    if (mime_type == extensions[i].mime_type)
      return extensions[i].suggested_extension;
  }
  return FILE_PATH_LITERAL("");
}
