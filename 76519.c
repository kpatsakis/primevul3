void AudioOutputController::DoClose(Task* closed_task) {
  DCHECK_EQ(message_loop_, MessageLoop::current());

  if (state_ != kClosed) {
    if (stream_) {
      stream_->Stop();
      stream_->Close();
      stream_ = NULL;
    }

    if (LowLatencyMode()) {
      sync_reader_->Close();
    }

    state_ = kClosed;
  }

  closed_task->Run();
  delete closed_task;
}
