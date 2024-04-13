void NetworkScreen::ButtonPressed(views::Button* sender,
                                  const views::Event& event) {
  ClearErrors();
  NetworkLibrary* network = CrosLibrary::Get()->GetNetworkLibrary();
  if (network && network->Connected()) {
    NotifyOnConnection();
  } else {
    continue_pressed_ = true;
    WaitForConnection(network_id_);
  }
}
