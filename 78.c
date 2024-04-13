  virtual void RemoveNetworkManagerObserver(NetworkManagerObserver* observer) {
    network_manager_observers_.RemoveObserver(observer);
  }
