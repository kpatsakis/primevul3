NetworkScreen::~NetworkScreen() {
  connection_timer_.Stop();
  UnsubscribeNetworkNotification();
}
