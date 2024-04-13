  static void ParseSystem(SystemInfo* system,
      EthernetNetwork** ethernet,
      WifiNetworkVector* wifi_networks,
      CellularNetworkVector* cellular_networks,
      WifiNetworkVector* remembered_wifi_networks) {
    DVLOG(1) << "ParseSystem:";
    DCHECK(!(*ethernet));
    for (int i = 0; i < system->service_size; i++) {
      const ServiceInfo* service = system->GetServiceInfo(i);
      DVLOG(1) << "  (" << service->type << ") " << service->name
               << " mode=" << service->mode
               << " state=" << service->state
               << " sec=" << service->security
               << " req=" << service->passphrase_required
               << " pass=" << service->passphrase
               << " id=" << service->identity
               << " certpath=" << service->cert_path
               << " str=" << service->strength
               << " fav=" << service->favorite
               << " auto=" << service->auto_connect
               << " is_active=" << service->is_active
               << " error=" << service->error;
      if (service->type == TYPE_ETHERNET)
        (*ethernet) = new EthernetNetwork(service);
      else if (service->type == TYPE_WIFI) {
        wifi_networks->push_back(new WifiNetwork(service));
      } else if (service->type == TYPE_CELLULAR) {
        cellular_networks->push_back(new CellularNetwork(service));
      }
    }

    if (!(*ethernet))
      (*ethernet) = new EthernetNetwork();

    DVLOG(1) << "Remembered networks:";
    for (int i = 0; i < system->remembered_service_size; i++) {
      const ServiceInfo* service = system->GetRememberedServiceInfo(i);
      if (service->auto_connect) {
        DVLOG(1) << "  (" << service->type << ") " << service->name
                 << " mode=" << service->mode
                 << " sec=" << service->security
                 << " pass=" << service->passphrase
                 << " id=" << service->identity
                 << " certpath=" << service->cert_path
                 << " auto=" << service->auto_connect;
        if (service->type == TYPE_WIFI) {
          remembered_wifi_networks->push_back(new WifiNetwork(service));
        }
      }
    }
  }
