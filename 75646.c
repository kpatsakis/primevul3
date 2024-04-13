static std::string FixupPath(const std::string& text) {
  DCHECK(!text.empty());

  FilePath::StringType filename;
#if defined(OS_WIN)
  FilePath input_path(UTF8ToWide(text));
  PrepareStringForFileOps(input_path, &filename);

  if (filename.length() > 1 && filename[1] == '|')
    filename[1] = ':';
#elif defined(OS_POSIX)
  FilePath input_path(text);
  PrepareStringForFileOps(input_path, &filename);
  if (filename.length() > 0 && filename[0] == '~')
    filename = FixupHomedir(filename);
#endif

  GURL file_url = net::FilePathToFileURL(FilePath(filename));
  if (file_url.is_valid()) {
    return UTF16ToUTF8(net::FormatUrl(file_url, std::string(),
        net::kFormatUrlOmitUsernamePassword, UnescapeRule::NORMAL, NULL,
        NULL, NULL));
  }

  return text;
}
