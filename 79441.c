  bool InitWithCommandLine(const CommandLine* cmd_line) {
    std::string channel_name =
        cmd_line->GetSwitchValueASCII(kDaemonIpcSwitchName);

#if defined(REMOTING_MULTI_PROCESS)
    if (channel_name.empty())
      return false;
#endif  // defined(REMOTING_MULTI_PROCESS)

    if (!channel_name.empty()) {
      daemon_channel_.reset(new IPC::ChannelProxy(
          channel_name, IPC::Channel::MODE_CLIENT, this,
          context_->network_task_runner()));
    }

#if !defined(REMOTING_MULTI_PROCESS)
    FilePath default_config_dir = remoting::GetConfigDir();
    host_config_path_ = default_config_dir.Append(kDefaultHostConfigFile);
    if (cmd_line->HasSwitch(kHostConfigSwitchName)) {
      host_config_path_ = cmd_line->GetSwitchValuePath(kHostConfigSwitchName);
    }
#endif  // !defined(REMOTING_MULTI_PROCESS)

    return true;
  }
