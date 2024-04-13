  void InitTestData() {
    ethernet_ = new EthernetNetwork();
    ethernet_->set_connected(true);
    ethernet_->set_service_path("eth1");

    STLDeleteElements(&wifi_networks_);
    wifi_networks_.clear();
    WifiNetwork* wifi1 = new WifiNetwork();
    wifi1->set_service_path("fw1");
    wifi1->set_name("Fake Wifi 1");
    wifi1->set_strength(90);
    wifi1->set_connected(false);
    wifi1->set_encryption(SECURITY_NONE);
    wifi_networks_.push_back(wifi1);

    WifiNetwork* wifi2 = new WifiNetwork();
    wifi2->set_service_path("fw2");
    wifi2->set_name("Fake Wifi 2");
    wifi2->set_strength(70);
    wifi2->set_connected(true);
    wifi2->set_encryption(SECURITY_WEP);
    wifi_networks_.push_back(wifi2);

    WifiNetwork* wifi3 = new WifiNetwork();
    wifi3->set_service_path("fw3");
    wifi3->set_name("Fake Wifi 3");
    wifi3->set_strength(50);
    wifi3->set_connected(false);
    wifi3->set_encryption(SECURITY_WEP);
    wifi_networks_.push_back(wifi3);

    wifi_ = wifi2;

    STLDeleteElements(&cellular_networks_);
    cellular_networks_.clear();

    CellularNetwork* cellular1 = new CellularNetwork();
    cellular1->set_service_path("fc1");
    cellular1->set_name("Fake Cellular 1");
    cellular1->set_strength(70);
    cellular1->set_connected(true);
    cellular1->set_activation_state(ACTIVATION_STATE_PARTIALLY_ACTIVATED);
    cellular1->set_payment_url(std::string("http://www.google.com"));
    cellular_networks_.push_back(cellular1);
    cellular_ = cellular1;

    remembered_wifi_networks_.clear();
    remembered_wifi_networks_.push_back(new WifiNetwork(*wifi2));

    int devices = (1 << TYPE_ETHERNET) | (1 << TYPE_WIFI) |
        (1 << TYPE_CELLULAR);
    available_devices_ = devices;
    enabled_devices_ = devices;
    connected_devices_ = devices;
    offline_mode_ = false;
  }
