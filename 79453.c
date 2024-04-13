  void Shutdown(int exit_code) {
    DCHECK(context_->network_task_runner()->BelongsToCurrentThread());

    if (shutting_down_)
      return;

    shutting_down_ = true;
    exit_code_ = exit_code;
    if (host_) {
      host_->Shutdown(base::Bind(
          &HostProcess::OnShutdownFinished, base::Unretained(this)));
    } else {
      OnShutdownFinished();
    }
  }
