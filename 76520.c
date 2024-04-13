void AudioOutputController::DoCreate(AudioParameters params) {
  DCHECK_EQ(message_loop_, MessageLoop::current());

  if (state_ == kClosed)
    return;
  DCHECK(state_ == kEmpty);

  if (!AudioManager::GetAudioManager())
    return;

  stream_ = AudioManager::GetAudioManager()->MakeAudioOutputStreamProxy(params);
  if (!stream_) {
    handler_->OnError(this, 0);
    return;
  }

  if (!stream_->Open()) {
    stream_->Close();
    stream_ = NULL;

    handler_->OnError(this, 0);
    return;
  }

  stream_->SetVolume(volume_);

  state_ = kCreated;

  handler_->OnCreated(this);

  if (!LowLatencyMode()) {
    AutoLock auto_lock(lock_);
    SubmitOnMoreData_Locked();
  }
}
