  void ResetHost() {
    DCHECK(context_->network_task_runner()->BelongsToCurrentThread());

    curtaining_host_observer_.reset();
    host_event_logger_.reset();
    log_to_server_.reset();
    heartbeat_sender_.reset();
    signaling_connector_.reset();
    signal_strategy_.reset();
    resizing_host_observer_.reset();
  }
