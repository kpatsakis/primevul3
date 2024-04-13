 bool PpapiPluginProcessHost::OnMessageReceived(const IPC::Message& msg) {
   bool handled = true;
   IPC_BEGIN_MESSAGE_MAP(PpapiPluginProcessHost, msg)
    IPC_MESSAGE_HANDLER(PpapiHostMsg_ChannelCreated,
                        OnRendererPluginChannelCreated)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  DCHECK(handled);
  return handled;
}
