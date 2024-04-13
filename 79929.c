bool FileEnumerator::IsDirectory(const FindInfo& info) {
  return S_ISDIR(info.stat.st_mode);
}
