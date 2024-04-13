  void ShutdownHostProcess() {
    DCHECK(context_->ui_task_runner()->BelongsToCurrentThread());

    config_watcher_.reset();
    daemon_channel_.reset();
    desktop_environment_factory_.reset();
    host_user_interface_.reset();

    if (policy_watcher_.get()) {
      base::WaitableEvent done_event(true, false);
      policy_watcher_->StopWatching(&done_event);
      done_event.Wait();
      policy_watcher_.reset();
    }

    context_.reset();
  }
