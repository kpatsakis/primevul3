  explicit SeekableIOBuffer(int capacity)
    : IOBuffer(capacity),
      real_data_(data_),
      capacity_(capacity),
      size_(0),
      used_(0) {
  }
