  void NotifyNetworkChanged(Network* network) {
    DCHECK(network);
    NetworkObserverMap::const_iterator iter = network_observers_.find(
        network->service_path());
    if (iter != network_observers_.end()) {
      FOR_EACH_OBSERVER(NetworkObserver,
                        *(iter->second),
                        OnNetworkChanged(this, network));
    } else {
      NOTREACHED() <<
          "There weren't supposed to be any property change observers of " <<
           network->service_path();
    }
  }
