void ChromotingInstance::PauseAudio(bool pause) {
  if (!IsConnected()) {
    return;
  }
  protocol::AudioControl audio_control;
  audio_control.set_enable(!pause);
  host_connection_->host_stub()->ControlAudio(audio_control);
}
