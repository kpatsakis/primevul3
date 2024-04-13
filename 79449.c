  void OnShutdownFinished() {
    DCHECK(context_->network_task_runner()->BelongsToCurrentThread());

    host_ = NULL;
    ResetHost();

    context_->ui_task_runner()->PostTask(
        FROM_HERE,
        base::Bind(&HostProcess::ShutdownHostProcess,
                   base::Unretained(this)));
  }
