void AudioOutputController::OnError(AudioOutputStream* stream, int code) {
  message_loop_->PostTask(
      FROM_HERE,
      NewRunnableMethod(this, &AudioOutputController::DoReportError, code));
}
