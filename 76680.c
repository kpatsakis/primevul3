bool PpapiPluginProcessHost::Send(IPC::Message* message) {
  return process_->Send(message);
}
