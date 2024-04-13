void ChromotingInstance::InjectKeyEvent(const protocol::KeyEvent& event) {
  if (IsConnected())
    input_tracker_.InjectKeyEvent(event);
}
