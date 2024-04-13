void AudioOutputController::Pause() {
  DCHECK(message_loop_);
  message_loop_->PostTask(
      FROM_HERE,
      NewRunnableMethod(this, &AudioOutputController::DoPause));
}
