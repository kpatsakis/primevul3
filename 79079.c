  virtual ~SeekableIOBuffer() {
    data_ = real_data_;
  }
