  void DidAppend(int bytes) {
    DCHECK_GE(bytes, 0);
    DCHECK_GE(size_ + bytes, 0);
    DCHECK_LE(size_ + bytes, capacity_);
    size_ += bytes;
  }
