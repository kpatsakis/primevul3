bool ReplaceFile(const FilePath& from_path, const FilePath& to_path) {
  base::ThreadRestrictions::AssertIOAllowed();
  return (rename(from_path.value().c_str(), to_path.value().c_str()) == 0);
}
