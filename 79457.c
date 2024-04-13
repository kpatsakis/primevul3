  void StartWatchingConfigChanges() {
#if !defined(REMOTING_MULTI_PROCESS)
    config_watcher_.reset(new ConfigFileWatcher(context_->ui_task_runner(),
                                                context_->file_task_runner(),
                                                this));
    config_watcher_->Watch(host_config_path_);
#endif  // !defined(REMOTING_MULTI_PROCESS)
  }
