void NetworkScreen::Refresh() {
  if (CrosLibrary::Get()->EnsureLoaded()) {
    SubscribeNetworkNotification();
    OnNetworkManagerChanged(chromeos::CrosLibrary::Get()->GetNetworkLibrary());
  }
}
