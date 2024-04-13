  void StartWatchingPolicy() {
    policy_watcher_.reset(
        policy_hack::PolicyWatcher::Create(context_->file_task_runner()));
    policy_watcher_->StartWatching(
        base::Bind(&HostProcess::OnPolicyUpdate, base::Unretained(this)));
  }
