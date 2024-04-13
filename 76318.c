void NetworkScreen::UpdateStatus(NetworkLibrary* network) {
  if (!view() || !network)
    return;

  if (network->Connected())
    ClearErrors();

  if (network->ethernet_connected()) {
    StopWaitingForConnection(
        l10n_util::GetStringUTF16(IDS_STATUSBAR_NETWORK_DEVICE_ETHERNET));
  } else if (network->wifi_connected()) {
    StopWaitingForConnection(ASCIIToUTF16(network->wifi_network()->name()));
  } else if (network->cellular_connected()) {
    StopWaitingForConnection(ASCIIToUTF16(network->cellular_network()->name()));
  } else if (network->ethernet_connecting()) {
    WaitForConnection(
        l10n_util::GetStringUTF16(IDS_STATUSBAR_NETWORK_DEVICE_ETHERNET));
  } else if (network->wifi_connecting()) {
    WaitForConnection(ASCIIToUTF16(network->wifi_network()->name()));
  } else if (network->cellular_connecting()) {
    WaitForConnection(ASCIIToUTF16(network->cellular_network()->name()));
  } else {
    StopWaitingForConnection(network_id_);
  }
}
