  virtual bool OnMessageReceived(const IPC::Message& message) {
    DCHECK(context_->ui_task_runner()->BelongsToCurrentThread());

#if defined(REMOTING_MULTI_PROCESS)
    bool handled = true;
    IPC_BEGIN_MESSAGE_MAP(HostProcess, message)
        IPC_MESSAGE_HANDLER(ChromotingDaemonNetworkMsg_Configuration,
                            OnConfigUpdated)
        IPC_MESSAGE_UNHANDLED(handled = false)
    IPC_END_MESSAGE_MAP()
    return handled;
#else  // !defined(REMOTING_MULTI_PROCESS)
    return false;
#endif  // !defined(REMOTING_MULTI_PROCESS)
  }
