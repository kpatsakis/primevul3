void PpapiPluginProcessHost::OnRendererPluginChannelCreated(
    const IPC::ChannelHandle& channel_handle) {
  if (sent_requests_.empty())
    return;

  Client* client = sent_requests_.front();
  sent_requests_.pop();

  client->OnPpapiChannelOpened(channel_handle, process_->GetData().id);
}
