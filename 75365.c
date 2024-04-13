bool PluginObserver::OnMessageReceived(const IPC::Message& message) {
  IPC_BEGIN_MESSAGE_MAP(PluginObserver, message)
    IPC_MESSAGE_HANDLER(ViewHostMsg_MissingPluginStatus, OnMissingPluginStatus)
    IPC_MESSAGE_HANDLER(ViewHostMsg_CrashedPlugin, OnCrashedPlugin)
    IPC_MESSAGE_HANDLER(ViewHostMsg_BlockedOutdatedPlugin,
                        OnBlockedOutdatedPlugin)
    IPC_MESSAGE_UNHANDLED(return false)
  IPC_END_MESSAGE_MAP()

  return true;
}
