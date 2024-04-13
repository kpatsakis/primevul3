void AudioOutputController::Flush() {
  DCHECK(message_loop_);
  message_loop_->PostTask(
      FROM_HERE,
      NewRunnableMethod(this, &AudioOutputController::DoFlush));
}
