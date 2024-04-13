MemoryMappedFile::MemoryMappedFile()
    : file_(base::kInvalidPlatformFileValue),
      data_(NULL),
      length_(0) {
}
