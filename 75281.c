void FilePathWatcherImpl::WillDestroyCurrentMessageLoop() {
  CancelOnMessageLoopThread();
}
