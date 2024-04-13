void DelegatedFrameHost::EndFrameSubscription() {
  idle_frame_subscriber_textures_.clear();
  frame_subscriber_.reset();
}
