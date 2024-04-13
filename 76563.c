void ChromotingInstance::PauseVideo(bool pause) {
  if (!IsConnected()) {
    return;
  }
  protocol::VideoControl video_control;
  video_control.set_enable(!pause);
  host_connection_->host_stub()->ControlVideo(video_control);
}
