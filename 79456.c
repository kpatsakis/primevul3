  void StartHostProcess() {
    DCHECK(context_->ui_task_runner()->BelongsToCurrentThread());

    if (!InitWithCommandLine(CommandLine::ForCurrentProcess())) {
      OnConfigWatcherError();
      return;
    }

    context_->network_task_runner()->PostTask(
        FROM_HERE,
        base::Bind(&HostProcess::ListenForShutdownSignal,
                   base::Unretained(this)));

    StartWatchingConfigChanges();
  }
