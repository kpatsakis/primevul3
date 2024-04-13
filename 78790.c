SharedMemory::SharedMemory()
    : mapped_file_(-1),
      inode_(0),
      mapped_size_(0),
      memory_(NULL),
      read_only_(false),
      requested_size_(0) {
}
