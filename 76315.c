void NetworkScreen::StopWaitingForConnection(const string16& network_id) {
  NetworkLibrary* network = CrosLibrary::Get()->GetNetworkLibrary();
  bool is_connected = network && network->Connected();
  if (is_connected && continue_pressed_) {
    NotifyOnConnection();
    return;
  }

  continue_pressed_ = false;
  connection_timer_.Stop();

  network_id_ = network_id;
  view()->ShowConnectingStatus(false, network_id_);
  view()->EnableContinue(is_connected);
}
