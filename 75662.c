static bool ValidPathForFile(const FilePath::StringType& text,
                             FilePath* full_path) {
  FilePath file_path(text);
  if (!file_util::AbsolutePath(&file_path))
    return false;

  if (!file_util::PathExists(file_path))
    return false;

  *full_path = file_path;
  return true;
}
