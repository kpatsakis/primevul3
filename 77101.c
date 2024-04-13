  ScopedSendOnIOThread(int host_id, IPC::Message* msg)
      : host_id_(host_id),
        msg_(msg),
        cancelled_(false) {
  }
