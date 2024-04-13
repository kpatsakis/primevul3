bool SharedMemory::FilePathForMemoryName(const std::string& mem_name,
                                         FilePath* path) {
  DCHECK_EQ(std::string::npos, mem_name.find('/'));
  DCHECK_EQ(std::string::npos, mem_name.find('\0'));

  FilePath temp_dir;
  if (!file_util::GetShmemTempDir(&temp_dir, false))
    return false;

#if !defined(OS_MACOSX)
#if defined(GOOGLE_CHROME_BUILD)
  std::string name_base = std::string("com.google.Chrome");
#else
  std::string name_base = std::string("org.chromium.Chromium");
#endif
#else  // OS_MACOSX
  std::string name_base = std::string(base::mac::BaseBundleID());
#endif  // OS_MACOSX
  *path = temp_dir.AppendASCII(name_base + ".shmem." + mem_name);
  return true;
}
