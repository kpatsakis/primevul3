bool SharedMemory::Delete(const std::string& name) {
  FilePath path;
  if (!FilePathForMemoryName(name, &path))
    return false;

  if (file_util::PathExists(path)) {
    return base::Delete(path, false);
  }

  return true;
}
