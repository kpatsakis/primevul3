void NetworkScreen::UnsubscribeNetworkNotification() {
  if (is_network_subscribed_) {
    is_network_subscribed_ = false;
    chromeos::CrosLibrary::Get()->GetNetworkLibrary()
        ->RemoveNetworkManagerObserver(this);
  }
}
