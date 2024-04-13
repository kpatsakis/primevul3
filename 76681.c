  ~PluginNetworkObserver() {
    net::NetworkChangeNotifier::RemoveConnectionTypeObserver(this);
    net::NetworkChangeNotifier::RemoveIPAddressObserver(this);
  }
