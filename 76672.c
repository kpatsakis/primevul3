void PpapiPluginProcessHost::OnChannelConnected(int32 peer_pid) {
  Send(new PpapiMsg_LoadPlugin(plugin_path_));

  for (size_t i = 0; i < pending_requests_.size(); i++)
    RequestPluginChannel(pending_requests_[i]);
  pending_requests_.clear();
}
