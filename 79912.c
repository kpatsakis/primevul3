bool CreateNewTempDirectory(const FilePath::StringType& prefix,
                            FilePath* new_temp_path) {
  FilePath tmpdir;
  if (!GetTempDir(&tmpdir))
    return false;

  return CreateTemporaryDirInDirImpl(tmpdir, TempFileName(), new_temp_path);
}
