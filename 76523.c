void AudioOutputController::DoSetVolume(double volume) {
  DCHECK_EQ(message_loop_, MessageLoop::current());

  volume_ = volume;

  if (state_ != kPlaying && state_ != kPaused && state_ != kCreated)
    return;

  stream_->SetVolume(volume_);
}
