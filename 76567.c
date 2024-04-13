void ChromotingInstance::ReleaseAllKeys() {
  if (IsConnected())
    input_tracker_.ReleaseAll();
}
