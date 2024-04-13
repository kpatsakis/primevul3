void AudioOutputController::DoReportError(int code) {
  DCHECK_EQ(message_loop_, MessageLoop::current());
  if (state_ != kClosed)
    handler_->OnError(this, code);
}
