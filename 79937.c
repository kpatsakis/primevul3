bool PathIsWritable(const FilePath& path) {
  base::ThreadRestrictions::AssertIOAllowed();
  return access(path.value().c_str(), W_OK) == 0;
}
