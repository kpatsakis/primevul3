  void DidConsume(int bytes) {
    SetOffset(used_ + bytes);
  }
