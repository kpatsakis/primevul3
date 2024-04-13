void AudioOutputController::EnqueueData(const uint8* data, uint32 size) {
  AutoLock auto_lock(lock_);
  pending_request_ = false;
  if (size) {
    buffer_.Append(data, size);
    SubmitOnMoreData_Locked();
  }
}
