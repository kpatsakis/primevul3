bool AbsolutePath(FilePath* path) {
  base::ThreadRestrictions::AssertIOAllowed();  // For realpath().
  char full_path[PATH_MAX];
  if (realpath(path->value().c_str(), full_path) == NULL)
    return false;
  *path = FilePath(full_path);
  return true;
}
