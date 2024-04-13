uint32 AudioOutputController::OnMoreData(
    AudioOutputStream* stream, uint8* dest,
    uint32 max_size, AudioBuffersState buffers_state) {
  if (!sync_reader_) {
    AutoLock auto_lock(lock_);

    buffers_state_ = buffers_state;

    if (state_ != kPlaying) {
      return 0;
    }

    uint32 size = buffer_.Read(dest, max_size);
    buffers_state_.pending_bytes += size;
    SubmitOnMoreData_Locked();
    return size;
  }

  uint32 size =  sync_reader_->Read(dest, max_size);
  sync_reader_->UpdatePendingBytes(buffers_state.total_bytes() + size);
  return size;
}
