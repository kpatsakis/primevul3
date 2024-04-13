uint32 SavePackage::GetMaxPathLengthForDirectory(const FilePath& base_dir) {
#if defined(OS_POSIX)
  return std::min(kMaxFilePathLength,
                  static_cast<uint32>(base_dir.value().length()) +
                  NAME_MAX + 1);
#else
  return kMaxFilePathLength;
#endif
}
