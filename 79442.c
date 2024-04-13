  void ListenForShutdownSignal() {
#if defined(OS_POSIX)
    remoting::RegisterSignalHandler(
        SIGTERM,
        base::Bind(&HostProcess::SigTermHandler, base::Unretained(this)));
#endif  // OS_POSIX
  }
