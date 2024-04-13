void AudioOutputController::SubmitOnMoreData_Locked() {
  lock_.AssertAcquired();

  if (buffer_.forward_bytes() > buffer_.forward_capacity())
    return;

  if (pending_request_)
    return;
  pending_request_ = true;

  AudioBuffersState buffers_state = buffers_state_;
  buffers_state.pending_bytes += buffer_.forward_bytes();

  AutoUnlock auto_unlock(lock_);
  handler_->OnMoreData(this, buffers_state);
}
