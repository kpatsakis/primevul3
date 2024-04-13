void AudioOutputController::DoPause() {
  DCHECK_EQ(message_loop_, MessageLoop::current());

  if (state_ != kPlaying)
    return;
  state_ = kPaused;

  stream_->Stop();

  if (LowLatencyMode()) {
    sync_reader_->UpdatePendingBytes(kPauseMark);
  }

  handler_->OnPaused(this);
}
