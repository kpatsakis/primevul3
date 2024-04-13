    NetworkObserverList(NetworkLibraryImpl* library,
                        const std::string& service_path) {
      network_monitor_ = MonitorNetworkService(&NetworkStatusChangedHandler,
                                               service_path.c_str(),
                                               library);
    }
