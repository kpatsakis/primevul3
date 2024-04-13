void Buffer::Unmap() {
  if (--map_count_ == 0)
    shm_.Unmap();
}
