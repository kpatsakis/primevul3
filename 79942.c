bool SetCurrentDirectory(const FilePath& path) {
  base::ThreadRestrictions::AssertIOAllowed();
  int ret = chdir(path.value().c_str());
  return !ret;
}
