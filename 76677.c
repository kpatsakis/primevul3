void PpapiPluginProcessHost::OpenChannelToPlugin(Client* client) {
  if (process_->GetHost()->IsChannelOpening()) {
    pending_requests_.push_back(client);
    return;
  }

  RequestPluginChannel(client);
}
