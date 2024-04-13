void RenderThreadImpl::OnNetworkTypeChanged(
    net::NetworkChangeNotifier::ConnectionType type) {
  EnsureWebKitInitialized();
  bool online = type != net::NetworkChangeNotifier::CONNECTION_NONE;
  WebNetworkStateNotifier::setOnLine(online);
  FOR_EACH_OBSERVER(
      RenderProcessObserver, observers_, NetworkStateChanged(online));
  WebNetworkStateNotifier::setWebConnectionType(
      NetConnectionTypeToWebConnectionType(type));
}
