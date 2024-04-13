void FilePathWatcherImpl::Cancel() {
  if (!delegate_) {
    set_cancelled();
    return;
  }

  if (!message_loop()->BelongsToCurrentThread()) {
    message_loop()->PostTask(FROM_HERE,
                             new FilePathWatcher::CancelTask(this));
  } else {
    CancelOnMessageLoopThread();
  }
}
