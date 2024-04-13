bool PpapiPluginProcessHost::Init(const content::PepperPluginInfo& info) {
  plugin_path_ = info.path;
  if (info.name.empty()) {
    process_->SetName(plugin_path_.BaseName().LossyDisplayName());
  } else {
    process_->SetName(UTF8ToUTF16(info.name));
  }

  std::string channel_id = process_->GetHost()->CreateChannel();
  if (channel_id.empty())
    return false;

  const CommandLine& browser_command_line = *CommandLine::ForCurrentProcess();
  CommandLine::StringType plugin_launcher =
      browser_command_line.GetSwitchValueNative(switches::kPpapiPluginLauncher);

#if defined(OS_LINUX)
  int flags = plugin_launcher.empty() ? ChildProcessHost::CHILD_ALLOW_SELF :
                                        ChildProcessHost::CHILD_NORMAL;
#else
  int flags = ChildProcessHost::CHILD_NORMAL;
#endif
  FilePath exe_path = ChildProcessHost::GetChildPath(flags);
  if (exe_path.empty())
    return false;

  CommandLine* cmd_line = new CommandLine(exe_path);
  cmd_line->AppendSwitchASCII(switches::kProcessType,
                              is_broker_ ? switches::kPpapiBrokerProcess
                                         : switches::kPpapiPluginProcess);
  cmd_line->AppendSwitchASCII(switches::kProcessChannelID, channel_id);

  static const char* kCommonForwardSwitches[] = {
    switches::kVModule
  };
  cmd_line->CopySwitchesFrom(browser_command_line, kCommonForwardSwitches,
                             arraysize(kCommonForwardSwitches));

  if (!is_broker_) {
    static const char* kPluginForwardSwitches[] = {
      switches::kNoSandbox,
      switches::kDisableSeccompFilterSandbox,
      switches::kPpapiFlashArgs,
      switches::kPpapiStartupDialog
    };
    cmd_line->CopySwitchesFrom(browser_command_line, kPluginForwardSwitches,
                               arraysize(kPluginForwardSwitches));
  }

  std::string locale =
      content::GetContentClient()->browser()->GetApplicationLocale();
  if (!locale.empty()) {
    cmd_line->AppendSwitchASCII(switches::kLang, locale);
  }

  if (!plugin_launcher.empty())
    cmd_line->PrependWrapper(plugin_launcher);

#if defined(OS_POSIX)
  bool use_zygote = !is_broker_ && plugin_launcher.empty() && info.is_sandboxed;
  if (!info.is_sandboxed)
    cmd_line->AppendSwitchASCII(switches::kNoSandbox, "");
#endif  // OS_POSIX
  process_->Launch(
#if defined(OS_WIN)
      FilePath(),
#elif defined(OS_POSIX)
      use_zygote,
      base::EnvironmentVector(),
#endif
      cmd_line);
  return true;
}
