bool MemoryMappedFile::MapFileToMemoryInternal() {
  base::ThreadRestrictions::AssertIOAllowed();

  struct stat file_stat;
  if (fstat(file_, &file_stat) == base::kInvalidPlatformFileValue) {
    DLOG(ERROR) << "Couldn't fstat " << file_ << ", errno " << errno;
    return false;
  }
  length_ = file_stat.st_size;

  data_ = static_cast<uint8*>(
      mmap(NULL, length_, PROT_READ, MAP_SHARED, file_, 0));
  if (data_ == MAP_FAILED)
    DLOG(ERROR) << "Couldn't mmap " << file_ << ", errno " << errno;

  return data_ != MAP_FAILED;
}
