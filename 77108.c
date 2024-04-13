  ~ScopedSendOnIOThread() {
    if (!cancelled_) {
      BrowserThread::PostTask(BrowserThread::IO,
                              FROM_HERE,
                              base::Bind(&SendOnIOThreadTask,
                                         host_id_,
                                         msg_.release()));
    }
  }
