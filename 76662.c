bool PluginInfoMessageFilter::OnMessageReceived(const IPC::Message& message,
                                                bool* message_was_ok) {
  IPC_BEGIN_MESSAGE_MAP_EX(PluginInfoMessageFilter, message, *message_was_ok)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(ChromeViewHostMsg_GetPluginInfo,
                                    OnGetPluginInfo)
    IPC_MESSAGE_UNHANDLED(return false)
  IPC_END_MESSAGE_MAP()
  return true;
}
