  explicit PluginNetworkObserver(PpapiPluginProcessHost* process_host)
      : process_host_(process_host) {
    net::NetworkChangeNotifier::AddIPAddressObserver(this);
    net::NetworkChangeNotifier::AddConnectionTypeObserver(this);
  }
