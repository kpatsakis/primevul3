int CreateAndOpenFdForTemporaryFile(FilePath directory, FilePath* path) {
  base::ThreadRestrictions::AssertIOAllowed();  // For call to mkstemp().
  *path = directory.Append(TempFileName());
  const std::string& tmpdir_string = path->value();
  char* buffer = const_cast<char*>(tmpdir_string.c_str());

  return HANDLE_EINTR(mkstemp(buffer));
}
